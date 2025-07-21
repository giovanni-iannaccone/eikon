# 🧩 Contributing
We welcome contributions! Please follow these steps:

1. Fork the repository.
2. Create a new branch ( using <a href="https://medium.com/@abhay.pixolo/naming-conventions-for-git-branches-a-cheatsheet-8549feca2534">this</a> convention).
3. Make your changes and commit them with descriptive messages.
4. Push your changes to your fork.
5. Create a pull request to the main repository.

Read <a href="/docs/">documentation</a> to know more about the actual implementation of WebWeaver

## 👨‍💻 Development guidelines
- **Modularity**: functions should be small and focused on a single responsibility
- **Separation of Concerns**: maintain clear separation between business logic and utility functions
- **Documentation**: add comments to explain complex logic, particularly when working with external libraries
- **Avoid external libraries**: try to keep raccoon as lightweight as possible, implement everything from scratch

## 🎨 Code style
- **Naming**: use classic C++ naming rules, snake_case for variables and functions and PascalCase for classes
- **Clarity and Consistency**: keep variable and function names descriptive, avoid unnecessary abbreviations
- **Formatting**: ensure your code is properly formatted
- **Imports**: organize imports into three sections:
    1. Standard library packages
    2. Internal project packages

Ensure all your tests pass before submitting a PR, update documentation if your changes affect the program's behaviour

These guidelines are designed to keep the codebase readable, maintainable, and consistent, even when multiple developers contribute. Following a common style helps reduce errors and makes the code easier to understand for others in the team.
If you'd like to add more specific guidelines or have questions, feel free to ask!

Thank you for contributing to raccoon! Your efforts are deeply appreciated. If you have any questions, feel free to reach out in the project discussions or open an issue.