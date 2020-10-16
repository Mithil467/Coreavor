# Contributing to Coreavor

👏🎉 First off, thanks for taking the time to contribute! 🎉👏

The following is a set of guidelines for contributing to Coreavor. These are mostly guidelines, not rules. Use your best judgment.

#### Table of Contents

[Build and Run](#build-and-run)

[Pull Requests](#pull-requests)

[Styleguides](#styleguides)
  - [Coding Style](#coding-style)
  - [Git Commit Messages](#git-commit-messages)

[Code of Conduct](#code-of-conduct)

## Build and Run

Install the following prerequisites for your operating system.
- Qt5 library
- C++ compiler

Clone the repo and run -
```
qmake -makefile
make
```
This will generate Coreavor binary executable.
You can install with
```
sudo make install
```

## Pull Requests

- All pull requests must have an issue linked with it. Please create an issue if it doesn't exist already before creating a pull request.
- Creating an issue beforehand helps discuss bugs and enhancements before work begins, preventing wasted effort.
- Comment on an issue if you are willing to work on it if no one else is assigned already.

1. Follow the [styleguides](#styleguides).

2. Single feature or bug-fix per PR.

3. Make your modification compact - don't reformat source code in your request. It makes code review more difficult.


## Styleguides

### Coding style

I recommend using [Qt Creator](https://www.qt.io/product/development-tools) to write code.

Code should be linted using clang-format. Make sure `clang-format` is installed and run `make clang-format` to automatically format code before each commit.

### Git Commit Messages

- Use the present tense ("Add feature" not "Added feature")
- Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
- Do not end the title with full-stop
- Limit the title to 72 characters or less
- Leave one line empty after your title if the message has a body
- Refer issues like "This fixes bug #ID" at the end of the body

## Code of Conduct

While contributing to the project, it is very important to maintain a healthy environment! Hence I request you to follow the [Code of Conduct](CODE_OF_CONDUCT).
