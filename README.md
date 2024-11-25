# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

# nov 25. addins by Zuzka
- main thing is i restructured the folders a bit to group functionality - added types dolder in srcs folder where functions that manipulate that data type are stores, feel free to restructure as u need, for me this made sense, is not done yet, just to make it a bit cleaner
- passes mini struct to a lot of functions because we could ont use the getenv function - since we have local array copy of envs, and here we add/remove new envs, the ghetenv onlky manipulated the system env array without our addins or changes - did getenv_local fucntion that has the same functionality
- so far it executes all ok, just prints one more error message line from the system together with our error from executor, must find hopw to stop it 

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
- HANDLE RELATIVE PATH IN THE GET_ENV_PATH = check first if we already maybe got a relative path, if not only then continue to searching the path in $PATH
- multiple terminals?
- check first for builtins,then relative path(starting with ./ or ../), then check absolute path (starting with /), then check $PATH
- cd builtin - check if path exists - relative, absolute, $PATH AND add error handling before
- group the functions in files better - eg. do files starting with type_... to store functions for out files e. type_cmd will store get_cmd_count, etc.
- heredoc: error handling, handle case with nothing after <<
- MAYBE?? for grep and possibly some other commands the trailing "" should stay as it then interprets it as a phrase
- cat -n leaks
- commands not found leak
- passing environment variable as first token big error eg. $BLA
- now we keep local array copy of envs, but we have to store it also to the shell envs as command may use it
// all builtins return 0 on success and 1 on error
// they do not quit the program in error, mut be handled by caller function
// TODO: must update exit status at end of each of these functions
- todo Zuzka: now we store envs as array, it will be much easier if we store them as linked list - redo export.c, env.c, and create getenv_local which we will use instead of getenv = not for future self: ACTUALLY NO BECAUSE EXECVE ACCEPTS AN ARRAY - KEEP IT IN ARRAY




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
