
# minishell
by [@vsanin42](https://www.github.com/vsanin42) & [@zuzanapiarova](https://www.github.com/zuzanapiarova)

A simple shell implementation project for 42 School.




## Use
```bash
  ./minishell
```


## Functionality

- Working command history and navigation with arrow keys
- Launching executables based on absolute/relative path or ```PATH``` variable
- Pipes ```|```, redirections: ```>, >>, <, <<```
- Builtins: ```echo, cd, pwd, export, unset, env, exit```
- Signal handling: ```SIGINT```, ```SIGQUIT```
- Handling of ```"```, ```'``` and respective enrivonment variable expansion
- Handling of ```$?``` (exit status code)

