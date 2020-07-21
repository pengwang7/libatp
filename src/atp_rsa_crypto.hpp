/*
 * MIT License
 *
 * Copyright (c) 2019 pengwang7(https://github.com/pengwang7/libatp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ATP_RSA_ENCRYPT_H__
#define __ATP_RSA_ENCRYPT_H__

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "glog/logging.h"

namespace atp {

typedef enum {
    RSA_ERROR_BASE             =  (-200),
    RSA_SET_PUBLIC_KEY_ERROR   =  (RSA_ERROR_BASE - 1),
    RSA_SET_PRIVATE_KEY_ERROR  =  (RSA_ERROR_BASE - 2),
    RSA_CHECK_FILE_EXIST_ERROR =  (RSA_ERROR_BASE - 3),
    RSA_READ_PUBLIC_KEY_ERROR  =  (RSA_ERROR_BASE - 4),
    RSA_READ_PRIVATE_KEY_ERROR =  (RSA_ERROR_BASE - 5),
    RSA_ENCRYPT_ERROR          =  (RSA_ERROR_BASE - 6),
    RSA_DECRYPT_ERROR          =  (RSA_ERROR_BASE - 7),
    RSA_INVALID_ARGS           =  (RSA_ERROR_BASE - 8),
    RSA_ENCRYPT_DATA_TO_LARGE  =  (RSA_ERROR_BASE - 9),
    RSA_TRANSFORM_ARGS_ERROR   =  (RSA_ERROR_BASE - 10),
    RSA_TRANSFORM_CERT_ERROR   =  (RSA_ERROR_BASE - 11),
    RSA_TRANSFORM_OPEN_ERROR   =  (RSA_ERROR_BASE - 12)
} RSAErrorCode;

/* The interface for message encrypt/decrypt. */
class RSA {
public:
    virtual int encryptContent(std::string content, int padding) = 0;

    virtual int decryptContent(std::string content, int origlen, int padding) = 0;
};

/*
 * The module implement RSA encrypt/decrypt.
 * Asymmetric RSA keys can encrypt/decrypt only data of limited length
 * i.e. RSAES-PKCS1-v1_5 encryption scheme defined in RFC3447 can operate on
 * messages of length up to k - 11 octets (k is the octet length of the RSA modulus)
 * so if you are using 2048-bit RSA key then maximum length of the plain data to be encrypted is 245 bytes.
 */
class RSACrypto : public RSA {
public:
    RSACrypto(std::string public_key = "", std::string private_key = "") {
        public_key_path_ = public_key;
        private_key_path_ = private_key;
    }

    ~RSACrypto() {

    }

public:
    const int DECRYPT_MAX_SIZE = (1024);

    int setPublicKeyPath(std::string public_key) {
        if (checkFileExist(public_key)) {
            return RSA_SET_PUBLIC_KEY_ERROR;
        }

        public_key_path_ = public_key;

        return 0;
    }

    int setPrivateKeyPath(std::string private_key) {
        if (checkFileExist(private_key)) {
            return RSA_SET_PRIVATE_KEY_ERROR;
        }

        private_key_path_ = private_key;

        return 0;
    }

    int encryptContent(std::string content, int padding) override {
        FILE* fp = fopen(public_key_path_.c_str(), "r");
        ::RSA* rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
        if (!rsa) {
            fclose(fp);
            LOG(ERROR) << "Read the RSA public key failed.";

            return RSA_READ_PUBLIC_KEY_ERROR;
        }

        int rsa_len = RSA_size(rsa);

        if (rsa_len < content.size()) {
            return RSA_ENCRYPT_DATA_TO_LARGE;
        }

        unsigned char chunk_buff[rsa_len + 1];
        unsigned char chunk_encrypt_buff[rsa_len];

        memset(chunk_buff, 0, sizeof(chunk_buff));
        memset(chunk_encrypt_buff, 0, sizeof(chunk_encrypt_buff));

        memcpy(chunk_buff, (const unsigned char*)(content.c_str()), rsa_len);

        int ret = RSA_public_encrypt(strlen((char*)chunk_buff), chunk_buff, chunk_encrypt_buff, rsa, padding == 0 ? RSA_NO_PADDING : padding);
        if (ret < 0) {
            char error[512];
            ERR_load_crypto_strings();
            ERR_error_string_n(ERR_get_error(), error, sizeof(error));
            LOG(ERROR) << "The RSA encrypt error: " << error;
        
            return -1;
        }

        encrypt_result_ = std::string((char*)chunk_encrypt_buff, ret);

        return 0;
    }

    int decryptContent(std::string content, int origlen, int padding) override {
        FILE* fp = fopen(private_key_path_.c_str(), "r");
        ::RSA* rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
        if (!rsa) {
            fclose(fp);
            LOG(ERROR) << "Read the RSA private key failed.";

            return RSA_READ_PRIVATE_KEY_ERROR;
        }

        int ret = 0;
        int rsa_len = RSA_size(rsa);
        int total_len = origlen / 4 * 3 + 2;

        if (total_len > rsa_len) {
            /* Support piecewise decryption. */
            int content_chunk_size = 0;

            if (total_len % rsa_len == 0) {
                content_chunk_size = total_len / rsa_len;
            } else {
                content_chunk_size = total_len / rsa_len + 1;
            }

            unsigned char chunk_buff[256 + 1];
            unsigned char chunk_decrypt_buff[256];
            for (int i = 0; i < content_chunk_size - 1; ++ i) {
                memset(chunk_buff, 0, sizeof(chunk_buff));
                memset(chunk_decrypt_buff, 0, sizeof(chunk_decrypt_buff));
                memcpy(chunk_buff, (const unsigned char*)(content.c_str() + i * rsa_len), rsa_len);

                ret = RSA_private_decrypt(rsa_len, chunk_buff, chunk_decrypt_buff, rsa, padding == 0 ? RSA_NO_PADDING : padding);
                if (ret < 0) {
                    fclose(fp);
                    char error[512];
                    ERR_load_crypto_strings();
                    ERR_error_string_n(ERR_get_error(), error, sizeof(error));
                    LOG(ERROR) << "The RSA decrypt error: " << error;

                    return RSA_DECRYPT_ERROR;
                }

                char tmp_buff[256];
                memset(tmp_buff, 0, sizeof(tmp_buff));
                memcpy(tmp_buff, chunk_decrypt_buff, ret);

                decrypt_result_ += std::string(tmp_buff);
            }
        } else {
            unsigned char decrypt_buff[DECRYPT_MAX_SIZE];
            memset(decrypt_buff, 0, sizeof(decrypt_buff));
            ret = RSA_private_decrypt(rsa_len, (const unsigned char*)content.c_str(), decrypt_buff, rsa, padding == 0 ? RSA_NO_PADDING : padding);
            if (ret < 0) {
                fclose(fp);
                char error[512];
                ERR_load_crypto_strings();
                ERR_error_string_n(ERR_get_error(), error, sizeof(error));
                LOG(ERROR) << "The RSA decrypt error: " << error;

                return RSA_DECRYPT_ERROR;
            }

            decrypt_result_ = std::string((char*)decrypt_buff);
        }

        RSA_free(rsa);
        fclose(fp);

        return 0;
    }

    int getPublicKeyFromCertificate(std::string cert_path, std::string public_key_path) {
        if (cert_path.size() == 0 || public_key_path.size() == 0) {
            return RSA_TRANSFORM_ARGS_ERROR;
        }

        FILE* input_cert_fp = fopen(cert_path.c_str(), "r");
        if (!input_cert_fp) {
            return RSA_TRANSFORM_OPEN_ERROR;
        }

        X509* cert = PEM_read_X509(input_cert_fp, NULL, NULL, NULL);
        if (!cert) {
            fclose(input_cert_fp);

            return RSA_TRANSFORM_CERT_ERROR;
        }

        // OpenSSL internal struct transform.
        EVP_PKEY* evp_key = X509_get_pubkey(cert);
        ::RSA* rsa = EVP_PKEY_get1_RSA(evp_key);

        FILE* public_key_fp = fopen(public_key_path.c_str(), "w+");
        if (!public_key_fp) {
            fclose(input_cert_fp);
            X509_free(cert);
            RSA_free(rsa);

            return RSA_TRANSFORM_OPEN_ERROR;
        }

        PEM_write_RSA_PUBKEY(public_key_fp, rsa);

        fclose(input_cert_fp);
        fclose(public_key_fp);

        X509_free(cert);
        RSA_free(rsa);

        return 0;
    }

    std::string getRsaEncryptResult() {
        return encrypt_result_;
    }

    std::string getRsaDecryptResult() {
        return decrypt_result_;
    }

private:
    int checkFileExist(std::string file) {
        FILE* fp = fopen(file.c_str(), "r");
        if (!fp) {
            return RSA_CHECK_FILE_EXIST_ERROR;
        }

        fclose(fp);

        return 0;
    }

private:
    std::string encrypt_result_;
    std::string decrypt_result_;
    std::string public_key_path_;
    std::string private_key_path_;
};

} /* end namespace atp */

#endif /* __ATP_RSA_ENCRYPT_H__ */
