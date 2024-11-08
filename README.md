# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIII
NUMBER OF TIMES WE CHANGED PARSER: I

# edge test cases
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???
- change double quotes lexing: because in input ${HOME}"aa" it should append the aa as text right after expanded home, but our will not because it is stored in a different node

# TODO
- lexer works but if we keep a space or other whitespace at the end of readline input it stores it as a separate node - but we can fix this
set stdin back to 0 when redirecting from files - maybe not if we use a different process
- in lexer I would add to each node information if there is whitespace before or if the new node is started because it is different type - good for parsing envs
- ! when there is text type before "" or '', then what si between quotes is not set to text type but keeps its type
- this should be the correct behaviour: echo $'HOME' --> HOME
- $'HOME' input segfaults - attempting free on address which was not malloced
- nested envs give error zsh: bad substitution
- error handling when unclosed quotes
- NOW IF ENVS ARE NOT ABLE TO BE EXPANDED A NULL VALUE/NOTHING IS RETURNED TO THE NODES VALUE, SO WE CAN HAVE VALUES OF TYPE TEXT BUT WITH NO STORED VALUE/NULL - HANDLE THIS - create a function that removes these nodes when processing them into the array
- !!! FILENAMES CAN ALSO BE ENVIRONMENT VARIABLES - must also go through the parse_eq_and_envs function !!!

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