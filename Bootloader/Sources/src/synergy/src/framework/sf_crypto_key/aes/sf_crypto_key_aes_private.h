/***********************************************************************************************************************
 * Copyright [2017-2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : sf_crypto_key_aes_private_api.h
 * Description  : SSP Crypto Key Framework module private API definition
 **********************************************************************************************************************/

#ifndef SF_CRYPTO_KEY_AES_PRIVATE_H
#define SF_CRYPTO_KEY_AES_PRIVATE_H

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER

/***********************************************************************************************************************
 * AES key type specific private functions. DO NOT USE! These functions are not for users but for Crypto Key
 * framework modules.
 **********************************************************************************************************************/
ssp_err_t sf_crypto_key_open_aes ( sf_crypto_key_instance_ctrl_t * p_ctrl, sf_crypto_key_cfg_t const * const p_cfg);

ssp_err_t sf_crypto_key_close_aes ( sf_crypto_key_instance_ctrl_t * p_ctrl);

ssp_err_t sf_crypto_key_generate_aes (sf_crypto_key_instance_ctrl_t * p_ctrl, sf_crypto_key_t * const p_secret_key);

void      sf_crypto_key_aes_interface_get (sf_crypto_key_instance_ctrl_t * const p_ctrl);

void sf_crypto_key_aes_get_available_api_interface(sf_crypto_key_instance_ctrl_t * const p_ctrl,
                                                   crypto_interface_get_param_t * p_param);

ssp_err_t sf_crypto_key_aes_verify_wrappedkey_buffersize(sf_crypto_key_size_t key_size, uint32_t buffer_length);

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif /* SF_CRYPTO_KEY_AES_PRIVATE_H */
