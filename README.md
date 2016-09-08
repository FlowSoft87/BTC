# BTC
Binary Tag Compound - A C++ include-only serialization library

The BTC library provides a simple way to serialize arbitrarily 
complicated data in binary form.
It defines the so-called BTagCompound object which is designed to 
work as a hashlist that stores string objects (the keys) together with 
data of primitive or BTagCompound type (the value).
In this way it is able to map even the most complicated structure.
Once constructed the whole structure can be serialized to an output
stream.
Instead of filling the structure by hand it can as well be deserialized
from an input stream.  
A basic working example is provided in the test section.
