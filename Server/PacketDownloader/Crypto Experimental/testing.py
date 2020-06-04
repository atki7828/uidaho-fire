from PythonCryptoWrapper import Crypto
from array import array
import array as arr

crypto = Crypto()

ciphertext = crypto.encrypt_cbc("test")

