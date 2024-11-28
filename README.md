# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

# nov 25.+26. addins by Zuzka
- did builtins: cd, pwd, env, export, unset, left: echo -n, exit is started but it has to accept a status code which we could keep stored somewhere but do not know how yet. and maybe set some signals idk you could have a look at it pls:D
- main thing is I restructured the folders a bit to group functionality - added types folder in srcs folder where functions that manipulate that data type are stores, feel free to restructure as u need, for me this made sense, is not done yet, just to make it a bit cleaner
- passed mini struct to a lot of functions because we could not use the getenv function - since we have local array copy of envs, and here we add/remove new envs, the getenv only manipulated the system env array without our addins or changes - did getenv_local function that has the same functionality as getenv but operates on our mini->env array
- generally works, just prints one more error message line from the system together with our error from executor, must find how to stop it
- muat do norm, now it is really bad:D and decide how to handle errors once and for all when we are together

# TODO
- major free function + error handling at all times
- handle heredoc <<
- set error codes properly - implement errno
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???
- also when env is the first text arg and it will not expand we do not have command to run  - what bash does it then goes to the next text element and stores this as command and the rest aas arguments
- pass struct mini to all functions that may need to quit the program
- also now when we have unclosed quotes or braces or non-alnum char in braces we exit the program but we could just print the error message and continue with waiting for input
- make it accept not only readline input but also get next line for reading from something - that's the non-interactive part. no idea how to test. isatty() function is used
- must check for pipes at start/end before starting to process input
- multiple terminals?
- static mode ?
- heredoc: error handling, handle case with nothing after <<
- MAYBE?? for grep and possibly some other commands the trailing "" should stay as it then interprets it as a phrase
// TODO: must update exit status at end of each of these functions
- signals error: when we run a command that waits for input eg. cat or sort, and press ^C, it will go bask to start of minishell IN THAT PROCESS so we get minishell > minishell >
- handle flags ?? if there are some, move it to shell implementation, if not use
-  < iaasdas alksdj  leaks
- when we have << at end it giver error "expected delimeter.." but it keeps the process opened and ^D does exit the innet one and the
- two pipes after each other leak in the child process
- sc error message shows cd and then cd again nstead of the non-existing folder
- check if there is actual cmd->cmd in cmd
- before evey exit free token list, cmd list, error message, mini->env
- append does not append 

- signal handling mini inside mini
## test cases
echo -nnnnnnnnnnnnnn hello

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
