# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIII
NUMBER OF TIMES WE CHANGED PARSER: I

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
- '"'"a"'"' causes memory leaks
- working on making lexer norm friendly

# edge test cases
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???

# TODO
- lexer works but if we keep a space or other whitespace at the end of readline input it stores it as a separate node - but we can fix this
- set stdin back to 0 when redirecting from files - maybe not if we use a different process
- error handling when unclosed quotes
- major free function + error handling at all times
- handle heredoc <<
- set error codes properly
- add WR end of the pipe and RD end of the pipe as file descriptors when we encounter a pipe


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
