# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

# 6.12. by Zuzka
- fixed redirection when we have single builtin executed in parent
- adjusted folder structure
- changed error messages to use perror and return errno where possible, must test still but for now looks good

# 6-7.12 by Vlad
- is *exit.c* in *srcs/* the same as in *srcs/utils/*? it's not compiled
- *export* with no arguments: i read about it on slack, some people just don't implement it as it's not specified. not really sure what it even does honestly, it's just the same output as env with '*declare -x*' before each env. maybe add this later for the sake of completeness.
- **ADDED:** heredoc via pipe in *set_files*
- **FIXED:** small *sigquit* issue which printed a message (it's just Quit now without core dump) + newline twice when it had to be done once
- **ADDED:** *echo* - has to print a newline when called with no arguments, this includes the case when can't expand an env (*echo $USERRR*)
- **FIXED:** *? echo* - with a workaround by returning 127 (command not found) and printing the message manually - not a perfect workaround because no other place can generate 127 errno naturally based on what i've seen and tried. $? gets set to 127 too. must keep this in mind when testing other things
- **FIXED:** issue with double unsets and single exports - same reason as expanding issue, strncmp against env_name len
- **ADDED:** *export* formatting - env name must be alnum and _ and cannot start with a number. if it's not the case - ERROR. on top of that it has to *do nothing* if there's no '=': export a does nothing and $? is 0
- **FIXED:** *env/pwd/... | grep "smth"* - prints the entire output, pipes from builtin to shell don't work - **ISSUE**: *cmd_list* passed to exec builtin instead of *nthcmd*, executing only the cmd head and not being able to progress. replaced that and is_builtin checker to match it. classic programming experience when something doesn't work cause of less than one line of code :DDDDDDDD shoutout to o1 for finding this, i was going insane debugging it...
- **LEAK:** *pwd | grep "/"* - leak from readline/xmalloc as if the very first input wasn't freed even though we free it in lexer. happened exactly once, could not replicate, never seen again. no idea
- **FIXED:** *input evaluator* - bad substitution should work properly
- **FIXED:** *echo ${?}a* returns 0a like in bash, issue in expansion  
- **TODO:** all checks in input evaluator currently work just once, need to put them in a loop to iterate through the whole input string instead
- **TODO:** *echo $'USER'*
- **TODO:** *check_input* exit status setting, idea to add a function specifically for printing the message + setting exit status in mini since it's being done before lexer and there's no access to errno. these are all our custom constraints, like not interpreting unclosed quotes from the subject (except for bad substitution, error code 1), so we handle these however we want

# 2-4.12 by Vlad
- finished and fixed echo, -n and -nnn... handled
- all builtins norm fixed
- fixed an issue with $USERaaa not expanding correctly
- signals work +-: child processes have their own signal handling along with termios reset to make the terminal print the characters - check behaviour of ctrl+\ with open cat or grep - this is one of the eval sheet checks. ctrl c, ctrl d no issues. ctrl \ in bash causes a core dump and exits - for us i guess we shouldn't manually cause any core dumps so let's keep it as a simple exit.
TODO: more mini inside mini checks but it also works.
- in executor once the big while loop is done: int arrays don't have a null terminator - how would they separate 0 and '\0'? - so i think we don't have to set the last element to 0. if we don't traverse it like char * - while (*str != '\0') then no need for this (pids[i] = '\0';)
- in unset: replaced 'if (!result') with 'if (!result[*i])': i assume we need to check each strdup result not the head of the array
- status codes: after the big executor loop added the correct conversion from 'status' received from waitpid to mini->exit_status.
- status now expands, issues with appending extra chars persist. works with and without {}
- invalid inputs - ${} can't start with numbers, nothing except _ allowed - done but a lot of checks are needed - input_evaluator.c
TODO: exit status setting before execution: maybe set default exit status to 1 so it's always an error if something can't reach the execution phase? only change to 0 if it can reach the execution and change back to 1 if an error occurs
- ? echo can fail and in this case returns -1 and sets errno
- set perror in each builtin and executor like we did in cd
- unset hh unsets also h=.. because of strncmp only comparing without trailing \0
- leaks when we call minishell in minishell - found when was in minishell, called make, and ran minishell again

# TODO
- handle heredoc << - - heredoc does not work with commands, I think because they take input from stdin and not from another token
- set error codes properly - implement errno && must update exit status at end of each of these functions
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???
- multiple terminals?
- static mode ?
- signals error: when we run a command that waits for input eg. cat or sort, and press ^C, it will go bask to start of minishell IN THAT PROCESS so we get minishell > minishell >
- handle flags ?? if there are some, move it to shell implementation, if not use
-  < iaasdas alksdj  leaks
- when we have << at end it giver error "expected delimeter.." but it keeps the process opened and ^D does exit the innet one and the
- two pipes after each other leak in the child process
- sc error message shows cd and then cd again nstead of the non-existing folder
- check if there is actual cmd->cmd in cmd
- before evey exit free token list, cmd list, error message, mini->env
- add input and output redirection when calling just one builtin
- in unser what/why are the falgs 1, 2 in function unset_strdup ?
- signal handling mini inside mini
- i think I get now why some error messages are in bash are lowercase and some start uppercase - the uppercase ones are the perror messages from the system, the others are defined in bash i think :D :D
- some commands prolly builtins do not return errno but 0
- cd in pipe and then echo hi does not print hi but it should
- heredoc echo << EOF ... EOF exits program !
- still when we paste it leaves the 10 letters in there in white background, works fine 

- echo $USERaaa prints only user name, should print nothing/newline. ${USER}aaa works fine
- signal handling of processes like cat that wait for input
- expand $?
- does not properly update exit status

## test cases
echo -nnnnnnnnnnnnnn hello - WORKS -Vlad

mini: echo ${"USER"} -> USER(right brace)
bash: ${"USER"}: bad substitution - WORKS
**more checks for input evaluator + expansion
keep this in mind when testing expansion because it means if unchecked,
it trims ${""} incorrectly**

mini: echo $'USER' -> $USER
bash: echo $'USER' -> USER

mini: echo ${?}a -> 0
bash: echo ${?}a -> 0a - WORKS

# HEREDOC MAIN THINGS - VLAD
main things:
- started working on heredoc: major question is how and when it's called. i think our initial idea was to call it before lexer - probably impossible because it hasn't been tokenized yet and although it could work, it's hard when working with only a string of input. the catch is with the quotes (reasoning a couple bullets below): for the heredoc to process quotes around delimiter and to decide whether or not to expand the following $, it has to, well, have the quotes. since we trim the quotes and expand during lexer, we also need to start handling heredoc during lexer. we cannot take the result of lexer, traverse it and check for quotes simply because there are no quotes. we can't do the whole heredoc yet, the idea for now is to make the exception for the token that follows << that would ignore the quotes
- UPDATE: this idea currently works, any text token that's after << does not get expanded or get its quotes trimmed. now this can go into further processing
- WIP: main heredoc implementation

specifics and edge cases:
- "<< DELIMITER" should work even though it doesn't consider << a command (not a text type). in the token list it shows as << type 6, DELIMITER type 0, in the command the cmd name is null, and the DELIMITER is the redir file. DELIMITER being a redir file doesn't make sense, but we can use it to extract its value to compare against in the heredoc loop. it just has to do nothing since no command is specified, even if a command is inside the heredoc loop. so gotta handle this specific case
- "ls << EOF -la" will also apply the flags afterwards so << DELIMITER doesn't have to be the last thing it does
- << '$USER' and << "$USER" DON'T EXPAND from what i tried - BOTH as COMMANDS inside the heredoc loop AND DELIMITERS. examples at the bottom of readme
- for validator/evaluator, << cannot be followed by any redirs, pipes or nothing/newlines (tested in bash), seems to only work with a string delimiter which is nice
- there can, however, be several sequences of << delimiter << delimiter ... which is not nice
- heredoc not only reads regular input but can also read from scripts/files.. - non-interactive and we should use get next line for that, so two separate approaches

- heredoc generally works by overwriting the value of token after << with a colleceted string - GREAT (Zuzana)
- lexer get_token_list edits and fixes

- heredoc seems to work, signal handling realized, double free issues avoided
- just need to move some functions to other files
- tried to comment all functions so it's understandable later - <3 Zuzka

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
