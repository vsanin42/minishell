# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIII
NUMBER OF TIMES WE CHANGED PARSER: I

#n november 11 add-ins by Zuzka:
- added comments with quic overview above some functions so we have easier time when evaluating and forget sth
- passing struct mini to functions that can throw error so we can properly free all and exit program
- changed the error_msg function to accept the struct mini and also possibly two allocated strings which it will free so we save some lines, can pass in NULL if we do not have any string to free
- could we possibly add all the processing and freeing into the process_input funcion? so we have it all together one after another and it is more readable, so we do token_list = lexer() --> cmd-list = parser(token_list) --> free(token_list) --> ... -> free(cmd_list) so it is all together and in this function we can also assign all the values to the mini struct so we have all this functionality in one place
- why is there the if statement in minishelll.c ?
t_cmd	*process_input(char *input, t_mini *mini) // should be void
{
	mini->token_list = lexer(input);
*	if (mini->cmd_list)       ??? why if mini??? *
		mini->cmd_list =  parser(mini);
	return (mini->cmd_list); // testing
}
- only has some leaks because trim_quotes was double freeing when we had non-expandable envs, because in envs I was freeing some memory
- add WR end of the pipe and RD end of the pipe as file descriptors when we encounter a pipe

# november 8 evening final status: (you can delete after reading)
- cmd is freed in show_prompt(), token list is freed in parser()
- now we can do a clean exit without leaks (based on fsanitize address) when an input is successfully converted to the cmd struct
- tested on only one case so could fail somewhere else: a "$HOME" c < infile.txt | d > outfile.txt
- env functions were fixed - free old value of res before reassigning it with ft_strjoin. very tight on norm there
- random notes at the end of readme below

# edge test cases
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???

# TODO
- lexer works but if we keep a space or other whitespace at the end of readline input it stores it as a separate node - but we can fix this
- set stdin back to 0 when redirecting from files - maybe not if we use a different process
- error handling when unclosed quotes
- NOW IF ENVS ARE NOT ABLE TO BE EXPANDED A NULL VALUE/NOTHING IS RETURNED TO THE NODES VALUE, SO WE CAN HAVE VALUES OF TYPE TEXT BUT WITH NO STORED VALUE/NULL - HANDLE THIS - create a function that removes these nodes when processing them into the array
- major free function + error handling at all times
- someimes when we call it, it does not guie any output??? just prints the "commmands: " but nothing after and we have to make re / make fclean + make again
- handle heredoc <<

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
