# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

**TODO** from tester:

1.  echo $, echo "$", echo '$' should print dollar, other  with $ should not print anything ----- FIXED
2. echo -n -n -n should take all the options as one, not first as options and others as arguments ----- FIXED
3. remove_null_token check if can be improved ----- optional
4. ctrl c - exit status change? ----- FIXED

1.  echo $, echo "$", echo '$' should print dollar, other  with $ should not print anything
- also echo "$""$" should print $$, prints nothing
2. echo -n -n -n should take all the options as one, not first as options and others as arguments 
3. remove_null_token check if can be improved ----- optional
4. ctrl c - exit status change? ----- optional
5. our workaround with cat "" to put empty spaces in there, so if we create a empty file called " " (whitespace) it will read its contents :D :D 
6. some cases with envs and quotes after dollar but it is so specific we do not need to handle it anymore 
- $"$"$"USER""" --> prints USER but should print $USER
- echo $"$"$"$USER""" --> prints vsanin but should print $vsanin
- echo $"$"$USER"" --> prints vsanin but should $vsanin
- echo $'$'$USER'' prints totally wrong :D but as you said who wants to break minishelll they will find a way, plus we handled environment variables in the (more than) basic way
7. had errors when expanding envs - were not considering _ in env name - FIXED 
8. if there is pipe and hdoc in other command, it takes input from pipe not from << - DONE -found it, was one line fix but it is a big thing i think :D
9. cd - changes to home and prints cwd - DONE

# 15.12. by Vlad
- i don't understand env builtin, need explanation (no thought head empty)
- norm: file separation etc, just kept some comments just in case. **TODO:** evaluators norm
- input evaluator: unclosed quotes, braces, bad substitution - generally work, now for the whole input string and on a more solid approach
- **TODO:** lexer_quotes.c lines 43 and 48 - indent problem, it's always been like this and idk what it could break. it essentially does nothing **DONT CARE**
- **TODO:** pids[num_of_p] is a variable length array which is forbidden by norm. skull emoji x100  need to find out how to replace it and keep everything intact **FIXED**
- set_files function can return errno but it's not assigned to anything in executor. issue or ok?
- does errno have to be assigned to int result before being returned?
- exectutor returns mini->exit_status and assigns it to mini->exit_status xd


In Bash, exit codes (also known as return codes) are integers that indicate the outcome of a command or script. They range from 0 to 255 and are used to convey whether a command was successful or encountered an error.
POSSIBLE BASH EXIT CODES
0: Success = The command or script executed successfully without errors.
1: General error = A catch-all for general errors.
2: Misuse of shell builtins = Misuse of built-in shell commands, such as an invalid syntax.
126: Command invoked cannot execute = The command was found but could not be executed (e.g., permission denied).
127: Command not found = The specified command does not exist.
128: Invalid argument to exit = An exit code outside the range of 0-255 was used.
128 + signal number: Fatal error signal = If a command is terminated by a signal, the exit code is 128 plus the signal number. For example:
	130: Script terminated by Ctrl+C (SIGINT, signal 2).
	137: Termination due to SIGKILL.
	139: Segmentation fault (SIGSEGV).
	255: Exit status out of range or special case
Exit codes greater than 255 "wrap around" modulo 256, so 256 becomes 0, 257 becomes 1, and so on. This behavior often indicates an out-of-range or erroneous exit code.