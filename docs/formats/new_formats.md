# Implementing new formats

This guide outlines the essential requirements and best practices to implement new data formats efficiently, ensuring maintainability, reusability, and consistency across your codebase.

- **Why?** Encapsulating each format’s implementation in its own class prevents naming conflicts and allows multiple formats to share function names with identical semantics. This creates a clean, modular architecture that enhances code readability and usability.

- **How?** Review existing format implementations to see the pattern: they all extend a base class `FormatHandler` and override virtual methods while creating private new ones. 

- **Key Principle**: Functions across formats should behave consistently in name and effect. If a format requires unique behavior or terminology, it should be set as private and invoked through common base functions to preserve uniformity.

### Mandatory Core Functions for Every Format

Every format implementation must provide the following functions, serving as a contract to ensure compatibility and predictability:

| Function name | Responsibility |
|---------------|----------------|
| `extract_signature` |	Extract the signature segment from the data.|
| `is_valid_signature` | Validate the correctness of the signature.|
| `read` | Read and parse the full content of the format.|
| `save` | Save or serialize the data back into the format.|

These functions form the core API that your format implementation should expose.
They provide a consistent interface for users and other components interacting with different formats.
Functions should be designed to be stateless and side-effect free where possible, enhancing testability and reliability.

Each format must define a variable named signature_size to specify the exact size of its signature segment.
Each format should implement an Error enum to return proper error codes. 
```cpp
enum Error: int {
    NO_ERROR,
    INVALID_SIGNAURE,
    ...
};
```

### Format-Specific Functions

While this guide defines a standardized core set of functions that every format must implement to maintain uniformity and interoperability, real-world formats often require additional, format-specific functions to handle unique features or optimizations.
You are encouraged to implement any extra functions directly in the format's class as private methods.

### Additional Recommendations for Scalability and Maintainability

- **Documentation**: Clearly document each function’s purpose, expected inputs/outputs, error handling, and any format-specific quirks.
- **Error Handling**: Use robust error reporting mechanisms. Prefer exceptions or error codes consistently across all formats.
- **Testing**: Implement comprehensive unit tests for each format, covering both typical and edge cases.
- **Versioning Support**: If your format evolves over time, design your functions to support version checks and backward compatibility gracefully.

By following these enhanced guidelines, your format implementations will be robust, intuitive, and easy to maintain or extend as new requirements arise. It’s all about building a strong foundation that scales elegantly.
