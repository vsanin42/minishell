# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: II
NUMBER OF TIMES WE CHANGED PARSER: I	

# edge test cases
- how to handle unclosed brackets or unclosed quotes and pipe at the end of input - HEREDOC ???
- change double quotes lexing: because in input ${HOME}"aa" it should append the aa as text right after expanded home, but our will not because it is stored in a different node

# TODO
- set stdin back to 0 when redirecting from files - maybe not if we use a different process
- in lexer I would add to each node information if there is whitespace before or if the new node is started because it is different type - good for parsing envs
- ! when there is text type before "" or '', then what si between quotes is not set to text type but keeps its type
- this should be the correct behaviour: echo $'HOME' --> HOME
-  $'HOME' input segfaults - attempting free on address which was not malloced
