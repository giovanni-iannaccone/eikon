# Implementing new formats

This guide will cover basic requirements to implement new formats using a scalable and unique approach.

1. Put your format implementation in a namespace: 
look at other format files, they all have the same functions name but in different namespaces, this helps increase ease of use because the same function will do the same thing for each format. Obviously every format will have some specific terms, if thats the case, don't worry, you can use those functions but they need to be called by base functions.

Base functions every format must have are:
read_header, extract_signature, is_valid_signature, read, save, write_header, write_signature.

Each format must also implement a variable called signature_size, which will store format's specific signature size;