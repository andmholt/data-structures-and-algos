# rsa-encryption-decryption

This RSA encryption and decryption program uses big primes (not bigger than longs in this instance) and modular exponentiation to securely and quickly encrypt and decrypt messages.

### Formats and functions

##### make rsa executable
`make`

##### start program
`./rsa [p] [q]`

Where p and q are two somewhat big primes. The bigger the more secure, however in this program the primes cannot be bigger than longs. Real-world applications of RSA alogirthms utilize primes much, much larger.

##### encrypt
`ENCRYPT`
`[output file] [n] [message]`

Where [output file] is the file the message will be outputted to once the encyrption process is complete. [n] is a integer n such that the original p * q = n. [message] is the message to be encrypted, which may contain as many words as desired. Numbers or other symbols may not be used.

##### decrypt
`DECRYPT`
`[input file] [output file]`

Where [input file] is the file with the message to be decrypted, and [output file] is the file where the decrypted message is to be written to once the decypriton process is complete.

##### exit
`EXIT`