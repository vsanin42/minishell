# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

# 9.12. by Zuzka
- **FIXED:** exporting envs with no value, just with name ignored them, but we still have to store them and ignore them only in printing with env commandm because with export+no arguments we have to print it even thiugh it is just a name
- **FIXED:** when here is env with naem and value and we export the same name with another value, it was correctly overwriting it. but if we export the same name but with no value noe it has to ignore it and keep the env with value, even though we re-declared it without value
- **ADDED:** export with no arguments prints declare x before each argument, prints also envs stored that do not have value
- **FIXED:** there was still this error: unset hh unsets also h=.. because of strncmp only comparing without trailing \0 - now we compare based on the lenght of the longer one and should work, tested
- **FIXED:** echo $USER prints thiis??? - ZDOTDIR=/nfs/homes/zpiarova = another env ??? - it was because it stops at _ and prints form there - it matches USER_ZDOTDIR=/nfs/homes/zpiarova - fixed like error above
- **FIXED:** echo $? was printing the correct code but with ? at end - fixed

# TODO ENV EXPANSION
- **TODO:** *echo $'USER'*

# TODO HEREDOC
- **TODO:**  any builtin with heredoc exits minishell eg. pwd << EOF gets input from EOF, prints pwd, but then exits ??? other commands with heredoc are fine and also heredoc with builtins in pipes is ok, i cannot figure out why

# TODO SIGNALS AND SETUP OF MINISHELL/READLINE
- **TODO:** !!! when we run minishell in minishell and press ^C, sometimes it prints ^C correctly in line, sometimes ^C in a newline, sometimes ^C after two newlines etc - can you fix? I have no idea
- **TODO:** also can we handle the 10 characters with white background at start of input when we copy-paste ?
- signal handling of processes like cat that wait for input *not sure if this still has to be done, can be removed? -Zuzka*

# TODO ERROR CODES AND EXIT
- some commands prolly builtins do not return errno but 0
- set error codes properly - implement errno && must update exit status at end of each of these functions

# TODO OTHERS
- **TODO:** all checks in input evaluator currently work just once, need to put them in a loop to iterate through the whole input string instead
- **TODO:** *check_input* exit status setting, idea to add a function specifically for printing the message + setting exit status in mini since it's being done before lexer and there's no access to errno. these are all our custom constraints, like not interpreting unclosed quotes from the subject (except for bad substitution, error code 1), so we handle these however we want
- multiple terminals?
- static mode ?
- in unset what/why are the flags 1, 2 in function unset_strdup ?

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
