#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "task.h"

#include "uart.h"
#include "mpu6000.h"
#include "flash.h"
#include "sbus.h"
#include "timer.h"
#include "gps.h"
#include "usbd_cdc_if.h"
