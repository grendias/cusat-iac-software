/*
 * Copyright (C) 2016 Libre Space Foundation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include "iac.h"
#include "spi.h"


int iac_spi_init(const char *device, const iac_spi_init_params_t *params)
{
    int fd;

    /* Open SPI device */
    fd = open(device, O_RDWR);
    if (fd == -1) {
        perror("Unable to open SPI device");
        return(-1);
    }

    /* Set mode */
    if (ioctl(fd, SPI_IOC_WR_MODE, &params->mode) == -1) {
        perror("Unable to set SPI mode");
        return(-1);
    }

    /* Set bits per word */
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &params->bits_per_word) == -1) {
        perror("Unable to set SPI bits per word");
        return(-1);
    }

    /* Set maximum speed in Hz  */
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &params->max_speed_hz) == -1) {
        perror("Unable to set SPI maximum speed");
        return(-1);
    }

    return(fd);
}


int iac_spi_transfer(const int fd, uint8_t *buf, const uint32_t buf_siz)
{
    struct spi_ioc_transfer transfer;

    memset(&transfer, 0, sizeof(transfer));

    transfer.tx_buf = (uint64_t) buf;
    transfer.rx_buf = (uint64_t) buf;
    transfer.len = buf_siz;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) == -1) {
        perror("Unable to write ioctl");
        return IAC_FAILURE;
    }

    return IAC_SUCCESS;
}