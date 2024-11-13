# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

# november 11 add-ins by Zuzka:
- added comments with quick overview above some functions so we have easier time when evaluating and forget sth
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
- working on adding RD and WR end of pipe to redir struct
DOING: if it starts with pipe we should not store it as fd - or maybe it is good because now we know that command started with pipe because first cmd->node has RD redir when it should not have and we can quit


# TODO
- lexer works but if we keep a space or other whitespace at the end of readline input it stores it as a separate node - but we can fix this
- set stdin back to 0 when redirecting from files - maybe not if we use a different process
- major free function + error handling at all times
- handle heredoc <<
- set error codes properly
- add WR end of the pipe and RD end of the pipe as file descriptors when we encounter a pipe --> probably in parser_redir.c find_redirs function or maybe in new_cmd function or in parser function
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???
- now it is possible to get an empty argument in the cmd->args when env cannot be expanded to anything, if we pass this empty argument to an executable, it will not run - we have to go over the args list and remove empty/NULL arguments
- when envs do not expand it stores NULL value in the array of arguments - we have to go over it and remove these values because if there are NULL values in the cmd args we pass to execve it will not run !
- also when env is the first text arg and it will not expand we do not have command to run  - what bash does it then goes to the next text element and stores this as command and the rest aas arguments
- pass struct mini to all functions that may need to quit the program
- also now when we have unclosed quotes or braces or non-albnum char in braces we exit the program but we could just print the error message and continue with waiting for input


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
