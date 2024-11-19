# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester


# november 11 add-ins by Zuzka:
- added comments with quick overview above functions so we have easier time when evaluating and forget sth
- added testing.c where I added functions we can use during eval - print token_list and print command_list
- passing struct mini to functions that can throw error so we can properly free all and exit program
- changed the error_msg function to accept the struct mini and also possibly two allocated strings which it will free so we save some lines, can pass in NULL if we do not have any string to free
- could we possibly add all the processing and freeing into the process_input function? so we have it all together one after another and it is more readable, so we do token_list = lexer() --> cmd-list = parser(token_list) --> free(token_list) --> ... -> free(cmd_list) so it is all together and in this function we can also assign all the values to the mini struct so we have all this functionality in one place --> check process_input in minishell.c
- why is there this if statement in minishell.c ?
t_cmd	*process_input(char *input, t_mini *mini) // should be void
{
	mini->token_list = lexer(input);
*	if (mini->cmd_list)       ??? why if mini??? *
		mini->cmd_list =  parser(mini);
	return (mini->cmd_list); // testing
}
- had some leaks because we were double freeing when we had non-expandable(bad) envs, but its fixed
- '"'"a"'"' causes memory leaks - FIXED

# november 12 add-ins by Zuzka
- made lexer norm friendly - tough as hell, we have to test a lot if no errors appeared, but i didnt find any so far
- renamed some files to be grouped by functionality: env_quotes -> lexer_quotes, env -> lexer_env, env_quotes_utils -> lexer_quotes_utils, like we have with parser_redir, you can change it back if it doesnt make sense to you
- FIXED '"'"a"'"' causes memory leaks - it handles two different pairs of nested quotes but on 3rd pair it starts causing leaks : "'"a"'" - count_words was not working good because here were supposed to be 3 words: ", a, ", but it was not counting the " as words and just moved past them thus the array was alllocated to small - only 1 word for: a

# november 13 add-ins by Zuzka
- changed the error_msg functions so it takes a message string, but also the struct mini so it can free it and also possibly 2 strings whic we need to free, helps with norm, if no string to free just pass in NULL
- working on adding RD and WR end of pipe to redir struct:
- add WR end of the pipe and RD end of the pipe as file descriptors when we encounter a pipe --> probably in parser_redir.c find_redirs function or maybe in new_cmd function or in parser function - DONE
- if input starts /ends with pipe we store it in redirs but we can check if first cmd node has RD pipe in redirs OR last cmd node has pipe WR in redirs and know pipe was first/last element - storing it is good because now we know that command started with pipe and we know we can quit/give the prompt back
- DOING: passing struct mini to all functions that may quit the program - BUT WE PROBABLY SHOULD DO IT THAT THEY CLEAR TOKEN AND COMMAND LISTS AND GIVE THE PROMPT BACK
- STARTED EVALUATOR - there are things we have to do before execution, feel free to add more
- we have to fix lexer - how strings in arrray are counted when we have unclosed quotes - functions mainly find_words, process_env_and_text and also check_next_char,

# november 14 add-ins by Vlad
- token list nodes with NULL value (in fact it's not null but \0 since it originates from ft_strdup("")) are now removed before being passed to parser
- braces after $ and unclosed quotes are handled right after receiving input and before passing to lexer - keeping it for now or until we find a safe way of doing it in a heredoc style

# november 18 add-ins by Vlad
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

misc:
- at the end of readme i left chatgpt's recap of a slack thread about how people did heredoc to get an idea of all available approaches and their benefits. i'll do the simplest one with a temp file for now. our priorities are ease of implementation and debugging, we don't expect large inputs. a big plus is that we have unlink() function available so we can delete the file after use

# TODO
- lexer works but if we keep a space or other whitespace at the end of readline input it stores it as a separate node - but we can fix this
- set stdin back to 0 when redirecting from files - maybe not if we use a different process
- major free function + error handling at all times
- handle heredoc <<
- set error codes properly
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???
- now it is possible to get an empty argument in the cmd->args when env cannot be expanded to anything, if we pass this empty argument to an executable, it will not run - we have to go over the args list and remove empty/NULL arguments
- when envs do not expand it stores NULL value in the array of arguments - we have to go over it and remove these values because if there are NULL values in the cmd args we pass to execve it will not run !
- also when env is the first text arg and it will not expand we do not have command to run  - what bash does it then goes to the next text element and stores this as command and the rest aas arguments
- pass struct mini to all functions that may need to quit the program
- also now when we have unclosed quotes or braces or non-alnum char in braces we exit the program but we could just print the error message and continue with waiting for input
- show prompt function is called at end of error_msg but it doesnt free all and returns 1 and keeps the data and continues the program, but we should get back to start of the program back
- implement errno
- make it accept not only readline input but also get next line for reading from something - that's the non-interactive part. no idea how to test. isatty() function is used
- must check for pipes at start/end before starting to process input
- HANDLE RELATIVE PATH IN THE GET_ENV_PATH = check first if we already maybe got a relative path, if not only then continue to searching the path in $PATH

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

# heredoc examples:

edit: nevermind there's a lot of cases, better test for yourself. cat << EOF -> "$USER" -> EOF -> "vsanin", while echo "$USER" is vsanin no quotes. insane after all the work we've done on this. maybe just don't trim the quotes idk

$ cat << EOF
$: $USER (same for ${USER})
$: EOF
vsanin

$ cat << 'EOF' (same for "EOF")
$: $USER
$: EOF
$USER

vsanin@c2r4s5:~/42Core/minishell$ cat << $USER
$: vsanin
$: $USER
vsanin

vsanin@c2r4s5:~/42Core/minishell$ cat << "$USER"
$: vsanin
$: $USER
vsanin

vsanin@c2r4s5:~/42Core/minishell$ cat << '$USER'
$: vsanin
$: $USER
vsanin

vsanin@c2r4s5:~/42Core/minishell$ cat << ${USER}
$: vsanin
$: $USER
$: ${USER}
vsanin
vsanin

# chatgpt heredoc approach recap

Overview of Heredoc Implementation Approaches
Here are various ways to implement heredoc in a shell, each with distinct pros, cons, and implications:

1. Temporary File Approach
Process: Collect heredoc input line-by-line, write it to a temporary file, and use the file as input in the execution stage.
Pros:

Mimics Bash behavior closely.
Easy to manage and access with file descriptors.
Handles large heredoc inputs efficiently.
Avoids memory-related issues.
Cons:

Requires filesystem interaction, adding I/O overhead.
Needs cleanup logic to remove temporary files.
Potential security risks with file naming in shared environments.
Ease of Implementation: Moderate. Uses open(), write(), and unlink() for cleanup.

2. Pipe-Based Approach
Process: Use a pipe to store heredoc input temporarily. Write to one end in the heredoc process and read from the other end during execution.
Pros:

No need for temporary files.
Efficient for small to medium-sized heredoc inputs.
Works well in a pipeline where heredoc output is directly passed as input.
Cons:

Limited buffer size; large inputs can cause blocking or require careful buffering.
Managing pipes and dup2() is more complex, especially with multiple heredocs.
Ease of Implementation: Moderate to challenging. Relies on pipe() and dup2() system calls.

3. In-Memory Storage (Linked List or String Buffer)
Process: Store heredoc input in a linked list, array, or concatenated string buffer. Pass the in-memory data to the next stage.
Pros:

No filesystem or file descriptor management.
Suitable for small heredocs, avoiding external I/O.
Simplifies testing without side effects.
Cons:

Risk of high memory usage for large heredocs.
Increased complexity for managing input termination and output flow.
Ease of Implementation: Simple to moderate, depending on data structure.

4. GNL (get_next_line) with Immediate Piping
Process: Use get_next_line() to read heredoc input. Immediately write each line to the next process via pipes.
Pros:

Avoids the need for file I/O.
Simple to implement if a custom GNL is already available.
Works well for sequential input.
Cons:

Inconsistent with Bash’s behavior (since heredoc input isn’t fully buffered).
Not ideal for standalone heredocs, as data is lost after direct output.
Ease of Implementation: Moderate but requires proper edge-case handling.

5. Hybrid: Pipe for Execution, Buffer for Editing
Process: Store heredoc input in a buffer (linked list, array) for user interaction and editing. Once ready, pipe it to the next process.
Pros:

Combines flexibility of in-memory operations with pipe-based execution.
Efficient for mid-sized data.
Minimizes disk I/O while preserving heredoc behavior.
Cons:

Involves dual management of buffers and pipes.
Implementation complexity increases due to buffer-pipe handoff.
Ease of Implementation: Complex but offers a balance between performance and flexibility.

Key Considerations
Input Size:

Large heredoc inputs favor file-based approaches.
Small to medium heredocs work well with in-memory or pipe-based solutions.
Bash Compliance:

Temporary file and pipe-based approaches mimic Bash more closely.
Immediate piping may diverge from expected behavior.
Security:

File-based approaches introduce security concerns with file naming.
In-memory approaches mitigate these risks but increase RAM usage.
Ease of Testing and Debugging:

File-based solutions are simpler to debug as the data is persistent.
Pipes and in-memory storage require more intricate debugging strategies.
Optimal Solution
The pipe-based approach strikes a balance between performance, Bash-like behavior, and minimal I/O. However, temporary files provide a simpler, more robust solution for large data at the cost of potential I/O overhead. If maintaining lightweight memory usage is critical, a hybrid method could be the most versatile.
