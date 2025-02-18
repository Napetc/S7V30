/***************************************************************************//**
* \file cybsp.h
*
* \brief
* Basic API for setting up specific boards
*
********************************************************************************
* \copyright
* Copyright 2021 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation
*
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

#pragma once

#include "stddef.h"
#include "stdint.h"
#include "cybsp_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_WIFI_INTERFACE_TYPE        CYBSP_SDIO_INTERFACE

extern void*   App_malloc(uint32_t size);
extern void*   App_calloc(size_t num, size_t size);
extern void    App_free(void *block_ptr);


#define whd_mem_malloc  App_malloc
#define whd_mem_calloc  App_calloc
#define whd_mem_free    App_free


#ifdef __cplusplus
}
#endif /* __cplusplus */

