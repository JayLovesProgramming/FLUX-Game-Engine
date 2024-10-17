Both `#pragma once` and `#ifndef ... #define ... #endif` are used to prevent multiple inclusions of a header file in C++ programs, but they have some differences. Here’s a breakdown to help you decide which is better for your use case:

### `#pragma once`
- **Simplicity**: It’s simpler to use. You just need to add `#pragma once` at the top of your header file.
- **Less Error-Prone**: Since it doesn't rely on macros, there's no chance of typo-related errors that can occur with `#ifndef`.
- **Performance**: It can be faster in some compilers because the compiler can implement it more efficiently than traditional include guards.

### `#ifndef ... #define ... #endif`
- **Standardized**: It is part of the C++ standard and will work with any C++ compiler.
- **Flexibility**: You can have more complex conditional compilation scenarios if needed.
- **Compatibility**: Older compilers may not support `#pragma once`, although this is rare with modern compilers.

### Conclusion
- If you want simplicity and are using a modern C++ compiler, `#pragma once` is a great choice.
- If you need maximum compatibility or have complex include scenarios, stick with the `#ifndef ... #define ... #endif` style.

Given that you seem to be working on a project that might involve various systems, if you're using a modern development environment, you could safely use `#pragma once` for its simplicity. However, for maximum portability and to ensure compatibility with all compilers, the traditional include guard method (`#ifndef`) is the safer option.
