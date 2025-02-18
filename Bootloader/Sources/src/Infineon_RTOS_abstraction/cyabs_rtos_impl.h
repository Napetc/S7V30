/***************************************************************************//**
* \file cyabs_rtos_impl.h
*
* \brief
* Defines the Cypress RTOS Interface. Provides prototypes for functions that
* allow Cypress libraries to use RTOS resources such as threads, mutexes & timing
* functions in an abstract way. The APIs are implemented
* in the Port Layer RTOS interface which is specific to the RTOS in use.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "tx_api.h"


/* RTOS thread priority */
typedef enum
{
  CY_RTOS_PRIORITY_MIN,
  CY_RTOS_PRIORITY_LOW,
  CY_RTOS_PRIORITY_BELOWNORMAL,
  CY_RTOS_PRIORITY_NORMAL,
  CY_RTOS_PRIORITY_ABOVENORMAL,
  CY_RTOS_PRIORITY_HIGH,
  CY_RTOS_PRIORITY_REALTIME,
  CY_RTOS_PRIORITY_MAX
} cy_thread_priority_t;


typedef struct
{
    uint32_t     maxcount;
    TX_SEMAPHORE tx_semaphore;
}
cy_semaphore_t;


typedef struct
{
    ULONG* mem;
    // ThreadX buffer size is a power of 2 times word size,
    // this is used to prevent memory corruption when get message from queue.
    size_t   itemsize;
    TX_QUEUE tx_queue;
} cy_queue_t;


typedef TX_EVENT_FLAGS_GROUP   cy_event_t;        // RTOS definition of a event


typedef TX_THREAD             *cy_thread_t;       // RTOS definition of a thread handle
typedef int32_t                cy_rtos_error_t;   // RTOS definition of a error status
typedef uint32_t               cy_time_t;
typedef uint32_t               cy_thread_arg_t;
typedef TX_MUTEX               cy_mutex_t;

