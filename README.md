# TCRE

A simple regular expression library.

## API

```
int match(char *subject, char *pattern);
```

Returns 1 if a match is found, otherwise 0.

## Supported Regular Expression Syntax

| Pattern | Description                                            |
| ------- | ------------------------------------------------------ |
| `c`     | Matches any non-special character `c`.                 |
| `\c`    | Matches any character `c` (e.g. `\*` to match `*`).    |
| `.`     | Matches any character.                                 |
| `[abc]` | Matches any character inside the brackets.             |
| `[^xy]` | Matches any character not inside the brackets.         |
| `[A-Z]` | Matches any character in range `A` to `Z` (inclusive). |
| `^`     | Anchor matching at start of subject.                   |
| `$`     | Anchor matching at end of subject.                     |
| `?`     | Matches the preceding character 0 or 1 times.          |
| `*`     | Matches the preceding character 0 or more times.       |
| `+`     | Matches the preceding character 1 or more times.       |

### Character Classes

The following are recognized in regular expressions:

| Character Class |
| --------------- |
| `[:alnum:]`     |
| `[:alpha:]`     |
| `[:blank:]`     |
| `[:cntrl:]`     |
| `[:digit:]`     |
| `[:graph:]`     |
| `[:lower:]`     |
| `[:print:]`     |
| `[:punct:]`     |
| `[:space:]`     |
| `[:upper:]`     |
| `[:xdigit:]`    |

# License

MIT License: see `LICENSE.md` for details.
