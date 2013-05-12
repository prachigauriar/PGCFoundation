# PGCFoundation

PGCFoundation is an exercise in implementing a simplified object system with limited subtype polymorphism using C. It provides a variety of general purpose classes to simplify writing software in C. These classes use reference counting to perform manual garbage collection, and include wrappers for many C primitives, dynamically resizable strings and arrays, linked lists, and dictionaries. The system itself is heavily influenced by the Cocoa Foundation framework and CoreFoundation C library by Apple. 

I wrote PGCFoundation primarily to understand how to use structural subtyping as a basis for polymorphism in C. While Objective-C and C++ are much more practical if you want objects in C, Objective-C doesn’t have an easy-to-deploy, cross-platform standard library, and I don’t particularly care for C++. Plus, there’s a lot to learn from implementing this sort of thing from scratch, e.g., how to implement autorelease pools.

Objects in PGCFoundation are only polymorphic on seven functions: dealloc, description, copy, equals, hash, retain, and release. To make it more generally polymorphic, I think PGCClass would need to be implemented as a true object so that we could have true metaclasses. 

I don’t think using something like this makes sense for real software projects, but as I mentioned earlier, it’s a simple, fun learning project.

All code is licensed under the MIT license. Do with it as you will.

