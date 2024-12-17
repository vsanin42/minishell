# minishell

42 project by vsanin & zpiarova - work in progress
NUMBER OF TIMES WE CHANGED LEXER: IIIII
NUMBER OF TIMES WE CHANGED PARSER: I
- tester: https://github.com/LucasKuhn/minishell_tester

- **TODO** echo $'$USER' (optional)

**TODO** from tester:
1.  echo $, echo "$", echo '$' should print dollar, other  with $ should not print anything
2. export =, export =aaa should print not a valid identifier - DONE, tested
3. cd cannot have multiple arguments - DONE, test
4. env $PWD should be updated when we cd - DONE, test
5. echo -n -n -n should take all the options as one, not first as options and others as arguments 

# 15.12. by Vlad
- i don't understand env builtin, need explanation (no thought head empty)
- norm: file separation etc, just kept some comments just in case. **TODO:** evaluators norm
- input evaluator: unclosed quotes, braces, bad substitution - generally work, now for the whole input string and on a more solid approach
- **TODO:** lexer_quotes.c lines 43 and 48 - indent problem, it's always been like this and idk what it could break. it essentially does nothing **DONT CARE**
- **TODO:** pids[num_of_p] is a variable length array which is forbidden by norm. skull emoji x100  need to find out how to replace it and keep everything intact **FIXED**
- set_files function can return errno but it's not assigned to anything in executor. issue or ok?
- does errno have to be assigned to int result before being returned?
- exectutor returns mini->exit_status and assigns it to mini->exit_status xd

- minishell$ echo "" or more quotes - **LEAKS**
- cat "": mini - nothing, doesn't even reach printing token list debugger. bash - no such file or directory. related to leaks, something is happening with empty quotes
- signals broke again - **FIXED**

- **FIXED i commented out one if condition and it broke XD**
- minishell$ echo "'"
- '
- minishell$ echo a"'"
- a"" (bash gives a') :/ 

- **also FIXED**
- echo ${}
- bash: ${}: bad substitution

# 10.12. by Vlad
- tried to figure out how to color minishell prompt, found nothing nice unfortunately :( gnu readline manual suggests we can edit some config files or do something else via command line but not sure. gotta be thankful we had it green for 99% of the way!
- **FIXED:** signals inside minishell and child processes - should be working fine, also tested and made sure that quitting via signals doesn't mess with the result error code. try *cat | ls*, end cat with ctrl c, echo $? - should be 0 because it's the exit code of the *final* pipeline command, which is *ls* which runs successfully. at the same time we need to "document" that the signal occured in one of the commands (we need this to know what to print like SIGQUIT results in ^\Quit etc.), so this was done in set_exit_status function and related ones.
- **FIXED?** echo $'USER' - i'm almost certain we can't do this perfectly. this structure itself (ANSI-C quoting) is kinda separate - $'string' is used to manage escape chars, for example \n, \t,... don't get taken literally but get printed. we don't need to handle it based on the subject, but i also felt like it's wrong to just throw an error. so ideally we handle it nicely, just ignore the special chars, but there's a problem in how our expansion and quote trimming is setup. we're limited by the fact that our code works linearly: get_env_value_to_process assumes that we've already decided we want to expand, but what if there's a series of $"$"??? on top of that, it used to split the received text even before, so $ would be separate from 'USER', i think it's fixed. i found some workarounds, partly hard(shit)coded in an ugly way but at least something. if you want to dive deeper try these prompts and try to find a pattern in them :DDD

echo $"$"$"USER"""
$USER
echo $"$"$"$USER"""
$vsanin
echo $"$USER"
vsanin
echo $"$"$USER""
$vsanin
echo $'$USER'
$USER
echo $'$'$USER''

so what i currently got working is:
- $'USER', $"USER" - both just USER;
- $'$USER' - $USER
- $"$USER" - vsanin
past that i believe are extremely niche cases that only someone who hates us with a passion would test so 1. i hope there are no such people 2. i'm prepared to defend this by simply saying it's the nature of minishell, if someone wants to break it, they will, it cannot be perfect in the time and subject constraints that we have.

- **STILL TODO: OTHERS**

# 9.12. by Zuzka
--- CURRETNLY WORKING ON EXIT CODES -Z

# 9.12. + 10.12. by Zuzka
- **FIXED:** exporting envs with no value, just with name ignored them, but we still have to store them and ignore them only in printing with env commandm because with export+no arguments we have to print it even thiugh it is just a name
- **FIXED:** when here is env with naem and value and we export the same name with another value, it was correctly overwriting it. but if we export the same name but with no value noe it has to ignore it and keep the env with value, even though we re-declared it without value
- **ADDED:** export with no arguments prints declare x before each argument, prints also envs stored that do not have value
- **FIXED:** there was still this error: unset hh unsets also h=.. because of strncmp only comparing without trailing \0 - now we compare based on the lenght of the longer one and should work, tested
- **FIXED:** echo $USER prints thiis??? - ZDOTDIR=/nfs/homes/zpiarova = another env ??? - it was because it matched any sequence starting with specified env name - so it matched USER_ZDOTDIR=/nfs/homes/zpiarova and printed the rest- fixed like error above
- **FIXED:** echo $? without {} was printing the correct code but with ? at end - fixed
- **FIXED:** i removed the color changing and escaping characters and now it does not print the additional 10 characters with white background at start of input when we copy-paste, but maybe we can put it back somehow ??
- **FIXED:**  any builtin with heredoc exits minishell eg. pwd << EOF gets input from EOF, prints pwd, but then exits ??? other commands with heredoc are fine and also heredoc with builtins in pipes is ok - long debugging but found out we have to set stdin, stdout back to 0 and 1 after we are finished with pipes - termios was giving error code: Inappropriate ioctl for device, thats how i found out
- but now that we dup stdin, stdout back to 0 and 1 after writing to pipe the pipe is closed and does not work anymore so commands like cat are constantly waiting for input but it is already closed - i really idk what to do, but we have to do something in executor_files_pipes.c/-> set_files -> if redir->type == TOKEN_HEREDOC - do something with file descriptors - OKAY IT IS FIXED AND WORKS FOR BOTH TYPES OF COMMANDS - I ADDED DUPLICATE OF STDIN, STDOUT IN MAIN AND IT IS SET BACK TO 0 AND 1 AT BEGINNING OF EACH ITERATION OF SHOW_PROMPT :D :D
- **FIXED:** we had small errors in export and unset where it worked with eg. C1= when there is = at end of string

# TODO SIGNALS AND SETUP OF MINISHELL/READLINE
- **TODO:** !!! when we run minishell in minishell and press ^C, sometimes it prints ^C correctly in line, sometimes ^C in a newline, sometimes ^C after two newlines etc - can you fix? I have no idea **FIXED -V**
- signal handling of processes like cat that wait for input *not sure if this still has to be done, can be removed? -Zuzka* **yes -V**

# TODO ERROR CODES AND EXIT
- **TODO:** set error codes properly - implement errno && must update exit status at end of each of these functions - now some commands prolly builtins do not return errno but 0 or 1 or at least it is not consistent
- errno is error code based on which we can print error messages and find what went wrong in the system
- exit status is

In Bash, exit codes (also known as return codes) are integers that indicate the outcome of a command or script. They range from 0 to 255 and are used to convey whether a command was successful or encountered an error.
POSSIBLE BASH EXIT CODES
0: Success = The command or script executed successfully without errors.
1: General error = A catch-all for general errors.
2: Misuse of shell builtins = Misuse of built-in shell commands, such as an invalid syntax.
126: Command invoked cannot execute = The command was found but could not be executed (e.g., permission denied).
127: Command not found = The specified command does not exist.
128: Invalid argument to exit = An exit code outside the range of 0-255 was used.
128 + signal number: Fatal error signal = If a command is terminated by a signal, the exit code is 128 plus the signal number. For example:
	130: Script terminated by Ctrl+C (SIGINT, signal 2).
	137: Termination due to SIGKILL.
	139: Segmentation fault (SIGSEGV).
	255: Exit status out of range or special case
Exit codes greater than 255 "wrap around" modulo 256, so 256 becomes 0, 257 becomes 1, and so on. This behavior often indicates an out-of-range or erroneous exit code.


# TODO OTHERS
- **TODO:** all checks in input evaluator currently work just once, need to put them in a loop to iterate through the whole input string instead
- **TODO:** *check_input* exit status setting, idea to add a function specifically for printing the message + setting exit status in mini since it's being done before lexer and there's no access to errno. these are all our custom constraints, like not interpreting unclosed quotes from the subject (except for bad substitution, error code 1), so we handle these however we want
- multiple terminals?
- static mode ?

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

# TEST CASES

**1 - 52: builtins**

- echo
```bash
1. echo hello world
mini output:	hello world
bash output:	hello world
mini exit code:	0
bash exit code:	0
mini error: 	n/a
bash error: 	n/a
result: 		OK
```

```bash
2. echo "hello world"
mini output:	hello world
bash output:	hello world
mini exit code:	0
bash exit code:	0
mini error: 	n/a
bash error: 	n/a
result: 		OK
```

```bash
3. echo 'hello world'
mini output:	hello world
bash output:	hello world
mini exit code:	0
bash exit code:	0
mini error: 	n/a
bash error: 	n/a
result: 		OK
```

```bash
4. echo hello'world'
mini output:	helloworld
bash output:	helloworld
mini exit code:	0
bash exit code:	0
mini error: 	n/a
bash error: 	n/a
result: 		OK
```

```bash
5. echo hello""world
mini output:	helloworld
bash output:	helloworld
mini exit code:	0
bash exit code:	0
mini error: 	n/a
bash error: 	n/a
result: 		OK
```

```bash
6. echo ''
mini output: 	newline
bash output: 	newline
mini exit code: 0
bash exit code: 0
mini error: 	n/a
bash error: 	n/a
result:			OK
```

```bash
7. echo "$HOME"
mini output:	/home/vsanin
bash output:	/home/vsanin
mini exit code: 0
bash exit code: 0
mini error: 	n/a
bash error: 	n/a
result:			OK
```

```bash
8. echo '$HOME'
mini output:	$HOME
bash output:	$HOME
mini exit code: 0
bash exit code: 0
mini error: 	n/a
bash error: 	n/a
result:			OK
```

```bash
9. echo "aspas ->'"
mini output: 	aspas ->'
bash output: 	aspas ->'
mini exit code: 0
bash exit code: 0
mini error: 	n/a
bash error: 	n/a
result:			OK
```

```bash
10. echo "aspas -> ' "
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
11. echo 'aspas ->"'
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
12. echo 'aspas -> " '
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
13. echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<" - NOT IMPLEMENTED WILDCARDS
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
14. echo '> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<' - NOT IMPLEMENTED WILDCARDS
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
15. echo "exit_code ->$? user ->$USER home -> $HOME"
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
16. echo 'exit_code ->$? user ->$USER home -> $HOME'
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
17. echo "$"
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
18. echo '$'
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
19. echo $
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
20. echo $?
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
21. echo $?HELLO
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

- pwd
```bash
22. pwd
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
23. pwd a
mini output:
bash output:
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

- export
```bash
24: export hello 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
25: export HELLO=123 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
26: export A- 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
27: export HELLO=123 A 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
28: export HELLO="123 A-" 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
29: export hello world 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
30: export HELLO-=123 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
31: export = 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
32: export 123 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

- unset
```bash
33: unset 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
34: unset HELLO 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
35: unset HELLO1 HELLO2 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
36: unset HOME 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
37: unset PATH 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
38: unset SHELL 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

- cd
```bash
39: cd $PWD 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
40: cd $PWD hi 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
41: cd 123123 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

- exit
```bash
42: exit 123
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
43: exit 298
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
44: exit +100 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
45: exit "+100" 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
46: exit +"100" 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
47: exit -100 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
48: exit "-100" 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
49: exit -"100" 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
50: exit hello 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
51: exit 42 world 
mini exit code:
bash exit code:
mini error:
bash error:
result:
```

```bash
52:  
mini exit code:
bash exit code:
mini error:
bash error:
result:
```


53: env | sort | grep -v SHLVL | grep -v ^_ 
mini output:
bash output: (CHROME_DESKTOP=code-url-handler.desktop COLORTERM=truecolor DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/101639/bus DESKTOP_SESSION=ubuntu DISPLAY=:0 DOCKER_HOST=unix:///run/user/101639/docker.sock FT_HOOK_NAME=login-user FT_HOOK_PATHNAME=login-user.d GDK_BACKEND=x11 GDMSESSION=ubuntu GIO_LAUNCHED_DESKTOP_FILE_PID=60740 GIO_LAUNCHED_DESKTOP_FILE=/usr/share/applications/code.desktop GIT_ASKPASS=/usr/share/code/resources/app/extensions/git/dist/askpass.sh GJS_DEBUG_OUTPUT=stderr GJS_DEBUG_TOPICS=JS ERROR;JS LOG GNOME_DESKTOP_SESSION_ID=this-is-deprecated GPG_AGENT_INFO=/run/user/101639/gnupg/S.gpg-agent:0:1 GTK_IM_MODULE=ibus GTK_MODULES=gail:atk-bridge HOME=/home/vsanin INVOCATION_ID=1a4534ac967d44ad96f042d24f4a0cea JOURNAL_STREAM=8:338906 KRB5CCNAME=FILE:/tmp/krb5cc_101639_0svDOf LANG=en_US.UTF-8 LIBVIRT_DEFAULT_URI=qemu:///system LOGNAME=vsanin MANAGERPID=58900 OLDPWD=/home/vsanin/42Core/minishell ORIGINAL_XDG_CURRENT_DESKTOP=ubuntu:GNOME PATH=/home/vsanin/bin:/home/vsanin/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin PWD=/home/vsanin/42Core/minishell/minishell_tester QT_ACCESSIBILITY=1 QT_IM_MODULE=ibus SESSION_MANAGER=local/c2r7s6.42prague.com:@/tmp/.ICE-unix/59475,unix/c2r7s6.42prague.com:/tmp/.ICE-unix/59475 SHELL=/bin/zsh SSH_AGENT_LAUNCHER=gnome-keyring SSH_AUTH_SOCK=/run/user/101639/keyring/ssh SYSTEMD_EXEC_PID=59494 TERM_PROGRAM_VERSION=1.93.0 TERM_PROGRAM=vscode TERM=xterm-256color USER=vsanin USER_ZDOTDIR=/home/vsanin VSCODE_GIT_ASKPASS_EXTRA_ARGS= VSCODE_GIT_ASKPASS_MAIN=/usr/share/code/resources/app/extensions/git/dist/askpass-main.js VSCODE_GIT_ASKPASS_NODE=/usr/share/code/code VSCODE_GIT_IPC_HANDLE=/run/user/101639/vscode-git-ff06acad39.sock VSCODE_INJECTION=1 XAUTHORITY=/home/vsanin/.Xauthority XDG_CONFIG_DIRS=/etc/xdg/xdg-ubuntu:/etc/xdg XDG_CURRENT_DESKTOP=Unity XDG_DATA_DIRS=/usr/share/ubuntu:/usr/share/gnome:/home/vsanin/.local/share/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share:/var/lib/snapd/desktop XDG_GREETER_DATA_DIR=/var/lib/lightdm-data/vsanin XDG_MENU_PREFIX=gnome- XDG_RUNTIME_DIR=/run/user/101639 XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0 XDG_SESSION_CLASS=user XDG_SESSION_DESKTOP=ubuntu XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0 XDG_SESSION_TYPE=x11 XMODIFIERS=@im=ibus ZDOTDIR=/home/vsanin)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
54: cat ./test_files/infile_big | grep oi 
mini output:
bash output: (was going to happen next. First, she tried to look down and make out disappointment it was empty: she did not like to drop the jar for fear not going to do _that_ in a hurry. “No, I’ll look first,” she said, “and see whether it’s marked ‘_poison_’ or not”; for she had read bottle marked “poison,” it is almost certain to disagree with you, However, this bottle was _not_ marked “poison,” so Alice ventured to brightened up at the thought that she was now the right size for going waited for a few minutes to see if she was going to shrink any further: said Alice to herself, “in my going out altogether, like a candle. I After a while, finding that nothing more happened, she decided on going Rabbit came near her, she began, in a low, timid voice, “If you please, to repeat it, but her voice sounded hoarse and strange, and the words now about two feet high, and was going on shrinking rapidly: she soon dropped it hastily, just in time to avoid shrinking away altogether. “Not like cats!” cried the Mouse, in a shrill, passionate voice. “Would trembling voice, “Let us get to the shore, and then I’ll tell you my “What I was going to say,” said the Dodo in an offended tone, “was, “But who is to give the prizes?” quite a chorus of voices asked. “Why, _she_, of course,” said the Dodo, pointing to Alice with one The next thing was to eat the comfits: this caused some noise and the others all joined in chorus, “Yes, please do!” but the Mouse only doesn’t suit my throat!” and a Canary called out in a trembling voice doing out here? Run home this moment, and fetch me a pair of gloves and at once in the direction it pointed to, without trying to explain the “How queer it seems,” Alice said to herself, “to be going messages for gloves, and was just going to leave the room, when her eye fell upon a she heard a voice outside, and stopped to listen. “Mary Ann! Mary Ann!” said the voice. “Fetch me my gloves this moment!” Next came an angry voice—the Rabbit’s—“Pat! Pat! Where are you?” And then a voice she had never heard before, “Sure then I’m here! Digging rumbling of little cartwheels, and the sound of a good many voices all then the Rabbit’s voice along—“Catch him, you by the hedge!” then silence, and then another confusion of voices—“Hold up his head—Brandy Last came a little feeble, squeaking voice, (“That’s Bill,” thought “We must burn the house down!” said the Rabbit’s voice; and Alice addressed her in a languid, sleepy voice. came different!” Alice replied in a very melancholy voice. By the use of this ointment—one shilling the box— going to dive in among the leaves, which she found to be nothing but Pigeon, raising its voice to a shriek, “and just as I was thinking I going to be, from one minute to another! However, I’ve got back to my are; secondly, because they’re making such a noise inside, no one could noise going on within—a constant howling and sneezing, and every now “Oh, _please_ mind what you’re doing!” cried Alice, jumping up and down (In which the cook and the baby joined): ear and left foot, so as to prevent its undoing itself,) she carried it No, there were no tears. “If you’re going to turn into a pig, my dear,” eyes, “Of course, of course; just what I was going to remark myself.” quarrelled last March—just before _he_ went mad, you know—” (pointing hoarse, feeble voice: “I heard every word you fellows were saying.” The Dormouse had closed its eyes by this time, and was going off into a neither of the others took the least notice of her going, though she voice, “Why the fact is, you see, Miss, this here ought to have been a you see, Miss, we’re doing our best, afore she comes, to—” At this “And who are _these?_” said the Queen, pointing to the three gardeners “Get up!” said the Queen, in a shrill, loud voice, and the three turning to the rose-tree, she went on, “What _have_ you been doing “May it please your Majesty,” said Two, in a very humble tone, going “Come on, then!” roared the Queen, and Alice joined the procession, “It’s—it’s a very fine day!” said a timid voice at her side. She was “Get to your places!” shouted the Queen in a voice of thunder, and had got its neck nicely straightened out, and was going to give the going to begin again, it was very provoking to find that the hedgehog “How do you like the Queen?” said the Cat in a low voice. “Who _are_ you talking to?” said the King, going up to Alice, and Alice thought she might as well go back, and see how the game was going on, as she heard the Queen’s voice in the distance, screaming with a large crowd collected round it: there was a dispute going on between thing before, and he wasn’t going to begin at _his_ time of life. startled when she heard her voice close to her ear. “You’re thinking “The game’s going on rather better now,” she said, by way of keeping up But here, to Alice’s great surprise, the Duchess’s voice died away, “A fine day, your Majesty!” the Duchess began in a low, weak voice. about half no time! Take your choice!” The Duchess took her choice, and was gone in a moment. As they walked off together, Alice heard the King say in a low voice, sea. The master was an old Turtle—we used to call him Tortoise—” “Why did you call him Tortoise, if he wasn’t one?” Alice asked. “We called him Tortoise because he taught us,” said the Mock Turtle Fainting in Coils.” two sobs choked his voice. “Same as if he had a bone in his throat,” the back. At last the Mock Turtle recovered his voice, and, with tears “Change lobsters again!” yelled the Gryphon at the top of its voice. Turtle, suddenly dropping his voice; and the two creatures, who had “There’s a porpoise close behind us, and he’s treading on my tail. They are waiting on the shingle—will you come and join the dance? Will you, won’t you, will you, won’t you, will you join the dance? Will you, won’t you, will you, won’t you, won’t you join the dance? Said he thanked the whiting kindly, but he would not join the dance. Would not, could not, would not, could not, would not join the dance. Would not, could not, would not, could not, could not join the dance. Then turn not pale, beloved snail, but come and join the dance. Will you, won’t you, will you, won’t you, will you join the dance? Will you, won’t you, will you, won’t you, won’t you join the dance?” “Boots and shoes under the sea,” the Gryphon went on in a deep voice, running on the song, “I’d have said to the porpoise, ‘Keep back, wise fish would go anywhere without a porpoise.” and told me he was going a journey, I should say ‘With what porpoise?’” Alice a little timidly: “but it’s no use going back to yesterday, “Stand up and repeat ‘’_Tis the voice of the sluggard_,’” said the “’Tis the voice of the Lobster; I heard him declare, His voice has a timid and tremulous sound.] wrong, and she went on in a trembling voice:— The Mock Turtle sighed deeply, and began, in a voice sometimes choked they doing?” Alice whispered to the Gryphon. “They can’t have anything “Stupid things!” Alice began in a loud, indignant voice, but she “I’m a poor man, your Majesty,” the Hatter began, in a trembling voice, The King looked anxiously at the White Rabbit, who said in a low voice, nearly out of sight, he said in a deep voice, “What are tarts made of?” “Treacle,” said a sleepy voice behind her. little voice, the name “Alice!” “The trial cannot proceed,” said the King in a very grave voice, “until verdict,” he said to the jury, in a low, trembling voice. “Why, there they are!” said the King triumphantly, pointing to the “Off with her head!” the Queen shouted at the top of her voice. Nobody looking up into hers—she could hear the very tones of her voice, and shared their never-ending meal, and the shrill voice of the Queen cries to the voice of the shepherd boy—and the sneeze of the baby, the shriek of the Gryphon, and all the other queer noises, would change unenforceability of any provision of this agreement shall not void the)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
55: cat minishell.h | grep ");"$ 
mini exit code::bash exit code = 1
56: export GHOST=123 | env | grep GHOST 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ()

———————————— redirects
57: grep hi <./test_files/infile 
mini output:
bash output: (hi)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
58: grep hi "<infile" <         ./test_files/infile 
mini exit code::bash exit code = 2
59: echo hi < ./test_files/infile bye bye 
mini output:
bash output: (hi bye bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
60: grep hi <./test_files/infile_big <./test_files/infile 
mini output:
bash output: (hi)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
61: echo <"./test_files/infile" 
mini output:
bash output: (bonjour 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
62: cat <"./test_files/file name with spaces" 
mini output:
bash output: (😈 😈 😈 This will break your minishell 😈 😈 😈)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
63: cat <./test_files/infile_big ./test_files/infile 
mini output:
bash output: (hi hello world 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
64: cat <"1""2""3""4""5" 
mini exit code::bash exit code = 1
65: echo <"./test_files/infile" <missing <"./test_files/infile" 
mini exit code::bash exit code = 1
66: echo <missing <"./test_files/infile" <missing 
mini exit code::bash exit code = 1
67: cat <"./test_files/infile" 
mini output:
bash output: (hi hello world 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
68: echo <"./test_files/infile_big" | cat <"./test_files/infile" 
mini output:
bash output: (hi hello world 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
69: echo <"./test_files/infile_big" | cat "./test_files/infile" 
mini output:
bash output: (hi hello world 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
70: echo <"./test_files/infile_big" | echo <"./test_files/infile" 
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
71: echo hi | cat <"./test_files/infile" 
mini output:
bash output: (hi hello world 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
72: echo hi | cat "./test_files/infile" 
mini output:
bash output: (hi hello world 42)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
73: cat <"./test_files/infile" | echo hi 
mini output:
bash output: (hi)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ( Broken pipe)
74: cat <"./test_files/infile" | grep hello 
mini output:
bash output: (hello)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
75: cat <"./test_files/infile_big" | echo hi 
mini output:
bash output: (hi)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ( Broken pipe)
76: cat <missing 
mini exit code::bash exit code = 1
77: cat <missing | cat 
mini exit code::bash exit code = 0
78: cat <missing | echo oi 
mini output:
bash output: (oi)
mini exit code::bash exit code = 0
79: cat <missing | cat <"./test_files/infile" 
mini output:
bash output: (hi hello world 42)
mini exit code::bash exit code = 0
80: echo <123 <456 hi | echo 42 
mini output:
bash output: (42)
mini exit code::bash exit code = 0
81: ls >./outfiles/outfile01 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
82: ls >         ./outfiles/outfile01 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
83: echo hi >         ./outfiles/outfile01 bye 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
84: ls >./outfiles/outfile01 >./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
85: ls >./outfiles/outfile01 >./test_files/invalid_permission 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)

bash error = ( Permission denied)
86: ls >"./outfiles/outfile with spaces" 
Only in ./bash_outfiles: outfile with spaces
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
87: ls >"./outfiles/outfile""1""2""3""4""5" 
Only in ./bash_outfiles: outfile12345
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
88: ls >"./outfiles/outfile01" >./test_files/invalid_permission >"./outfiles/outfile02" 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)

bash error = ( Permission denied)
89: ls >./test_files/invalid_permission >"./outfiles/outfile01" >./test_files/invalid_permission 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)

bash error = ( Permission denied)
90: cat <"./test_files/infile" >"./outfiles/outfile01" 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
hello
world
42
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
91: echo hi >./outfiles/outfile01 | echo bye 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
92: echo hi >./outfiles/outfile01 >./outfiles/outfile02 | echo bye 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
93: echo hi | echo >./outfiles/outfile01 bye 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
94: echo hi | echo bye >./outfiles/outfile01 >./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
95: echo hi >./outfiles/outfile01 | echo bye >./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ()
96: echo hi >./outfiles/outfile01 >./test_files/invalid_permission | echo bye 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ( Permission denied)
97: echo hi >./test_files/invalid_permission | echo bye 
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ( Permission denied)
98: echo hi >./test_files/invalid_permission >./outfiles/outfile01 | echo bye 
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)

bash error = ( Permission denied)
99: echo hi | echo bye >./test_files/invalid_permission 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 100: ❌ echo hi | >./outfiles/outfile01 echo bye >./test_files/invalid_permission 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 101: ❌ echo hi | echo bye >./test_files/invalid_permission >./outfiles/outfile01 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 102: ❌ cat <"./test_files/infile" >./test_files/invalid_permission 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 103: ❌ cat >./test_files/invalid_permission <"./test_files/infile" 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 104: ❌ cat <missing >./outfiles/outfile01 
mini exit code:
bash exit code: 1
Test 105: ❌ cat >./outfiles/outfile01 <missing 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
Test 106: ❌ cat <missing >./test_files/invalid_permission 
mini exit code:
bash exit code: 1
Test 107: ❌ cat >./test_files/invalid_permission <missing 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 108: ❌ cat >./outfiles/outfile01 <missing >./test_files/invalid_permission 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
Test 109: ❌ ls >>./outfiles/outfile01 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 110: ❌ ls >>      ./outfiles/outfile01 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 111: ❌ ls >>./outfiles/outfile01 >./outfiles/outfile01 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 112: ❌ ls >./outfiles/outfile01 >>./outfiles/outfile01 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 113: ❌ ls >./outfiles/outfile01 >>./outfiles/outfile01 >./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 114: ❌ ls >>./outfiles/outfile01 >>./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 115: ❌ ls >>./test_files/invalid_permission 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 116: ❌ ls >>./test_files/invalid_permission >>./outfiles/outfile01 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 117: ❌ ls >>./outfiles/outfile01 >>./test_files/invalid_permission 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 118: ❌ ls >./outfiles/outfile01 >>./test_files/invalid_permission >>./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 119: ❌ ls <missing >>./test_files/invalid_permission >>./outfiles/outfile02 
mini exit code:
bash exit code: 1
Test 120: ❌ ls >>./test_files/invalid_permission >>./outfiles/outfile02 <missing 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 121: ❌ echo hi >>./outfiles/outfile01 | echo bye 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 122: ❌ echo hi >>./outfiles/outfile01 >>./outfiles/outfile02 | echo bye 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 123: ❌ echo hi | echo >>./outfiles/outfile01 bye 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 124: ❌ echo hi | echo bye >>./outfiles/outfile01 >>./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 125: ❌ echo hi >>./outfiles/outfile01 | echo bye >>./outfiles/outfile02 
Only in ./bash_outfiles: outfile01
Only in ./bash_outfiles: outfile02
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
hi
bye
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 126: ❌ echo hi >>./test_files/invalid_permission | echo bye 
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 127: ❌ echo hi >>./test_files/invalid_permission >./outfiles/outfile01 | echo bye 
mini output:
bash output: (bye)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 128: ❌ echo hi | echo bye >>./test_files/invalid_permission 
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 129: ❌ echo hi | echo >>./outfiles/outfile01 bye >./test_files/invalid_permission 
Only in ./bash_outfiles: outfile01
mini outfiles:
cat: './mini_outfiles/*': No such file or directory
bash outfiles:
mini exit code:
bash exit code: 1
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 130: ❌ cat <minishell.h>./outfiles/outfile 
mini exit code:
bash exit code: 1
Test 131: ❌ cat <minishell.h|ls 
mini output:
bash output: (bash_outfiles bash.supp bonus bonus_bonus builtins extras local.supp loop.out manual_tests mini_outfiles os_specific outfiles pipes README.md redirects syntax tester test_files wildcards)
mini exit code:
bash exit code: 0
———————————— extras
Test 132: ❌  
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 133: ❌ $PWD 
mini exit code:
bash exit code: 126
mini error = ( No such file or directory)
bash error = ( Is a directory)
Test 134: ❌ $EMPTY 
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 135: ❌ $EMPTY echo hi 
mini output:
bash output: (hi)
mini exit code:
bash exit code: 0
mini error = ( No such file or directory)
bash error = ()
Test 136: ❌ ./test_files/invalid_permission 
mini exit code:
bash exit code: 126
mini error = ( No such file or directory)
bash error = ( Permission denied)
Test 137: ❌ ./missing.out 
mini exit code:
bash exit code: 127
Test 138: ❌ missing.out 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 139: ❌ "aaa" 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 140: ❌ test_files 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 141: ❌ ./test_files 
mini exit code:
bash exit code: 126
mini error = ( No such file or directory)
bash error = ( Is a directory)
Test 142: ❌ /test_files 
mini exit code:
bash exit code: 127
Test 143: ❌ minishell.h 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 144: ❌ $ 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 145: ❌ $? 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 146: ❌ README.md 
mini exit code:
bash exit code: 127
mini error = ( No such file or directory)
bash error = ( command not found)