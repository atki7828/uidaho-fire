import ctypes

lib = ctypes.cdll.LoadLibrary('./libCrypto.so')

class Crypto(object):
    def __init__(self):
        self.obj = lib.Crypto_new()

    def encrypt_cbc(self, byteStr):
        return lib.Crypto_encrypt_cbc(self.obj, byteStr)

    def decrypt_cbc(self, byteStr):
        return lib.Crypto_decrypt_cbc(self.obj, byteStr)
