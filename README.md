# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

- **TODO** echo $'$USER' (optional)

# 15.12. by Vlad
- i don't understand env builtin, need explanation (no thought head empty)
- norm: file separation etc, just kept some comments just in case. **TODO:** evaluators norm
- input evaluator: unclosed quotes, braces, bad substitution - generally work, now for the whole input string and on a more solid approach
- **TODO:** lexer_quotes.c lines 43 and 48 - indent problem, it's always been like this and idk what it could break. it essentially does nothing **DONT CARE**
- **TODO:** pids[num_of_p] is a variable length array which is forbidden by norm. skull emoji x100  need to find out how to replace it and keep everything intact **FIXED**
- set_files function can return errno but it's not assigned to anything in executor. issue or ok?
- does errno have to be assigned to int result before being returned?
- exectutor returns mini->exit_status and assigns it to mini->exit_status xd

- minishell$ echo "" or more quotes - **LEAKS**
- cat "": mini - nothing, doesn't even reach printing token list debugger. bash - no such file or directory. related to leaks, something is happening with empty quotes
- signals broke again - **FIXED**

- **FIXED i commented out one if condition and it broke XD**
- minishell$ echo "'"
- '
- minishell$ echo a"'"
- a"" (bash gives a') :/ 

- **also FIXED**
- echo ${}
- bash: ${}: bad substitution

# 10.12. by Vlad
- tried to figure out how to color minishell prompt, found nothing nice unfortunately :( gnu readline manual suggests we can edit some config files or do something else via command line but not sure. gotta be thankful we had it green for 99% of the way!
- **FIXED:** signals inside minishell and child processes - should be working fine, also tested and made sure that quitting via signals doesn't mess with the result error code. try *cat | ls*, end cat with ctrl c, echo $? - should be 0 because it's the exit code of the *final* pipeline command, which is *ls* which runs successfully. at the same time we need to "document" that the signal occured in one of the commands (we need this to know what to print like SIGQUIT results in ^\Quit etc.), so this was done in set_exit_status function and related ones.
- **FIXED?** echo $'USER' - i'm almost certain we can't do this perfectly. this structure itself (ANSI-C quoting) is kinda separate - $'string' is used to manage escape chars, for example \n, \t,... don't get taken literally but get printed. we don't need to handle it based on the subject, but i also felt like it's wrong to just throw an error. so ideally we handle it nicely, just ignore the special chars, but there's a problem in how our expansion and quote trimming is setup. we're limited by the fact that our code works linearly: get_env_value_to_process assumes that we've already decided we want to expand, but what if there's a series of $"$"??? on top of that, it used to split the received text even before, so $ would be separate from 'USER', i think it's fixed. i found some workarounds, partly hard(shit)coded in an ugly way but at least something. if you want to dive deeper try these prompts and try to find a pattern in them :DDD

echo $"$"$"USER"""
$USER
echo $"$"$"$USER"""
$vsanin
echo $"$USER"
vsanin
echo $"$"$USER""
$vsanin
echo $'$USER'
$USER
echo $'$'$USER''

so what i currently got working is:
- $'USER', $"USER" - both just USER;
- $'$USER' - $USER
- $"$USER" - vsanin
past that i believe are extremely niche cases that only someone who hates us with a passion would test so 1. i hope there are no such people 2. i'm prepared to defend this by simply saying it's the nature of minishell, if someone wants to break it, they will, it cannot be perfect in the time and subject constraints that we have.

- **STILL TODO: OTHERS**

# 9.12. by Zuzka
--- CURRETNLY WORKING ON EXIT CODES -Z

# 9.12. + 10.12. by Zuzka
- **FIXED:** exporting envs with no value, just with name ignored them, but we still have to store them and ignore them only in printing with env commandm because with export+no arguments we have to print it even thiugh it is just a name
- **FIXED:** when here is env with naem and value and we export the same name with another value, it was correctly overwriting it. but if we export the same name but with no value noe it has to ignore it and keep the env with value, even though we re-declared it without value
- **ADDED:** export with no arguments prints declare x before each argument, prints also envs stored that do not have value
- **FIXED:** there was still this error: unset hh unsets also h=.. because of strncmp only comparing without trailing \0 - now we compare based on the lenght of the longer one and should work, tested
- **FIXED:** echo $USER prints thiis??? - ZDOTDIR=/nfs/homes/zpiarova = another env ??? - it was because it matched any sequence starting with specified env name - so it matched USER_ZDOTDIR=/nfs/homes/zpiarova and printed the rest- fixed like error above
- **FIXED:** echo $? without {} was printing the correct code but with ? at end - fixed
- **FIXED:** i removed the color changing and escaping characters and now it does not print the additional 10 characters with white background at start of input when we copy-paste, but maybe we can put it back somehow ??
- **FIXED:**  any builtin with heredoc exits minishell eg. pwd << EOF gets input from EOF, prints pwd, but then exits ??? other commands with heredoc are fine and also heredoc with builtins in pipes is ok - long debugging but found out we have to set stdin, stdout back to 0 and 1 after we are finished with pipes - termios was giving error code: Inappropriate ioctl for device, thats how i found out
- but now that we dup stdin, stdout back to 0 and 1 after writing to pipe the pipe is closed and does not work anymore so commands like cat are constantly waiting for input but it is already closed - i really idk what to do, but we have to do something in executor_files_pipes.c/-> set_files -> if redir->type == TOKEN_HEREDOC - do something with file descriptors - OKAY IT IS FIXED AND WORKS FOR BOTH TYPES OF COMMANDS - I ADDED DUPLICATE OF STDIN, STDOUT IN MAIN AND IT IS SET BACK TO 0 AND 1 AT BEGINNING OF EACH ITERATION OF SHOW_PROMPT :D :D
- **FIXED:** we had small errors in export and unset where it worked with eg. C1= when there is = at end of string

# TODO SIGNALS AND SETUP OF MINISHELL/READLINE
- **TODO:** !!! when we run minishell in minishell and press ^C, sometimes it prints ^C correctly in line, sometimes ^C in a newline, sometimes ^C after two newlines etc - can you fix? I have no idea **FIXED -V**
- signal handling of processes like cat that wait for input *not sure if this still has to be done, can be removed? -Zuzka* **yes -V**

# TODO ERROR CODES AND EXIT
- **TODO:** set error codes properly - implement errno && must update exit status at end of each of these functions - now some commands prolly builtins do not return errno but 0 or 1 or at least it is not consistent
- errno is error code based on which we can print error messages and find what went wrong in the system
- exit status is

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


# TODO OTHERS
- **TODO:** all checks in input evaluator currently work just once, need to put them in a loop to iterate through the whole input string instead
- **TODO:** *check_input* exit status setting, idea to add a function specifically for printing the message + setting exit status in mini since it's being done before lexer and there's no access to errno. these are all our custom constraints, like not interpreting unclosed quotes from the subject (except for bad substitution, error code 1), so we handle these however we want
- multiple terminals?
- static mode ?

## test cases
mini: echo ${"USER"} -> USER(right brace)
bash: ${"USER"}: bad substitution - WORKS
**more checks for input evaluator + expansion
keep this in mind when testing expansion because it means if unchecked,
it trims ${""} incorrectly**

mini: echo $'USER' -> $USER
bash: echo $'USER' -> USER

mini: echo ${?}a -> 0
bash: echo ${?}a -> 0a - WORKS

# general notes

1. assume:
char **head;
char **temp = head;

don't use (*temp)++ to move the pointer when accessing strings themselves, it changes the starting position of strings within head, causing corruption.

reason: temp is a separate pointer iterating over the array, so it doesn't modify head itself. However, the problem lies in the line:
(*temp)++;
This alters the actual char * string that temp points to, meaning you're modifying the original head's strings' pointers, not just moving through the array.

weird shit but it's fixed :D

2. do this when using strjoin to avoid leaks - free the old res value before reassigning (setting to NULL probably doesn't matter but anyway):
to_append = ft_substr(text, (*i) - len, len);
oldres = res;
res = ft_strjoin(res, to_append);
free(oldres);
free(to_append);
oldres = NULL;
to_append = NULL;

3. when freeing token list, changed the function to use a double pointer (t_token **token), so we pass in the address of original pointer. This allows you to modify the caller’s reference directly, setting it to NULL after freeing all nodes (tsken from GPT) so now we set the head of token list to NULL to avoid double frees - I dont understand it well yet but kinda makes sense
