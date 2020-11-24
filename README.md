<h1 id="finite-state-automation">Automata and formal languages tool (AFL)</h1>


![enter image description here](https://i.ibb.co/jfqRzQc/snapshot.png "AFL")


| Syntax:                          | Effect:                                                                      | Example:            |
|----------------------------------|------------------------------------------------------------------------------|---------------------|
| fsa [name]                       |  registers an automation with one state with this name.                      | fsa test1           |
| fsa [name] [regex]               | registers an automation with this name for this regex.                       | fsa test2 a*(a+b)*b |
| print [name]                     | prints the automation.                                                       | print test1         |
| accepts [fsa] [string]           | returns true if the fsa accepts the string and false otherwise               | accepts a1  aabbb   |
| add_state [fsa]                  | adds a new state to the automation                                           | add_state a1        |
| arc [fsa] [start] [end] [symbol] | adds a new transition from state start to state end with this symbol         | arc a1 0 1 a        |
| det [fsa]                        | fsa becomes deterministc                                                     | det a1              |
| [fsa1] = det [fsa]               | registers an automation fsa1 which is the deterministic fsa2                 | sa1 = det a         |
| min [fsa]                        | fsa becomes deterministic, total and minimal                                 | min a1              |
| [fsa1] = min [fsa]               | registers an automation fsa1 which is the deterministic and minimal fsa2     | a1 = min a2         |
| [fsa1] = union [fsa2] [fsa3]     | registers an automation fsa1 which is the union of fsa2 and fsa3             | a1 = union a2 a3    |
| [fsa1] = concat [fsa2] [fsa3]    | registers an automation fsa1 which is the concatenation of fsa2 and fsa3     | a1 = concat a2 a3   |
| [fsa1] = intersect [fsa2] [fsa3] | registers an automation fsa1 which is the intersection of fsa2 and fsa3      | a1 = intersect a2 a3|
| star [fsa1]                      | fsa1 becomes an new automation for the Kleene star of the old fsa1 automation| star a1             |
| [fsa1] = star [fsa2]             | registers an automation fsa1 which is the Kleene star of fsa2                | a1 = star a2        |
| compl [fsa1]                     | fsa1 becomes an new automation for the complement of the old fsa1 automation | compl a1            |
| [fsa1] = compl [fsa2]            | registers an automation fsa1 which is the complement of fsa2                 | a1 = compl a2       |
| reverse [fsa1]                   | fsa1 becomes an new automation for the reverse of the old fsa1 automation    | reverse a1          |
| [fsa1] = reverse [fsa2]          | registers an automation fsa1 which is the reverse of fsa2                    | a1 = reverse a2     |
| regex [fsa1]                     | returns a regex for the automation.                                          | regex a1            |


<h1 id="finite-state-automation">1. Finite State Automation</h1>

![enter image description here](https://i.ibb.co/FKQgLBg/69973577-352536238957668-4630865521305190400-n.png "example of FSA")


<p>Finite state automation is A = &lt;Q,Σ,s,F,δ&gt;, where<br>
Q - is a finite, non-empty set of states<br>
Σ- is the input alphabet (a finite, non-empty set of symbols).<br>
s - is an initial state, an element of Q<br>
F- is the set of final states, a  subset of Q<br>
δ- is the state-transition function</p>
<h1 id="creation">Creation</h1>
<p>Diffrent ways of creating an automation:</p>

```c++
int main()
{

	//With  regular expression (Have to use many brackets for now) *fix later
	// Automation for a(a+b)*
	FiniteStateAutomation A("a(a+b)*");

	cout << A.accepts("abbb") << endl; //true;
	cout << A.accepts("bbba") << endl; //false

	FiniteStateAutomation A2; //Only one state with index 0
	
	A2.addState(); //Adds state with index 1
	A2.addTransition(0, 1, 'a');
	A2.addTransition(1, 1, 'a');
	A2.addTransition(1, 1, 'b');


	return 0;
}
```


<h2 id="control">Control</h2>

<table>
<thead>
<tr>
<th></th>
<th></th>
</tr>
</thead>
<tbody>
<tr>
<td>AddState()</td>
<td>Adds a new state. Returns the index of the new state.</td>
</tr>
<tr>
<td>AddTransition(start,end,ch)</td>
<td>Creates a new transition from state start to state end with the letter ch. May become from DFA to NFA.</td>
</tr>
<tr>
<td>ChangeStartState(newStart)</td>
<td>Changes the start state of the automation. Returns false if the given new start state doesn’t exist and true if it was successful.</td>
</tr>
<tr>
<td>MakeStateFinal(state)</td>
<td>Marks the given state as final. Returns false if the state doesn’t exist.</td>
</tr>
<tr>
<td>Accepts(word)</td>
<td>Returns true if the word is accepted by the DFA/NFA.</td>
</tr>
<tr>
<td>IsEmptyLanguage()</td>
<td>Returns true if the FSA doesn't accept any strings.</td>
</tr>
<tr>
<td>RemoveNotReachableStates()</td>
<td>Removes all states that are not reachable from the start state..</td>
</tr>	
</tbody>
</table><h2 id="properties">Properties</h2>

<table>
<thead>
<tr>
<th></th>
<th></th>
</tr>
</thead>
<tbody>
<tr>
<td>MakeTotal()</td>
<td>Makes the automation complete. It shoud define a transition for each state and each input symbol. We create an error state for every non-existing transition.</td>
</tr>
<tr>
<td>MakeDeterministic()</td>
<td>Makes the Non-deterministic finite automaton(NFA)  to Deterministic finite Automation(DFA).</td>
</tr>
<tr>
<td>Minimize()</td>
<td>Transforms the given DETERMINISTIC finite automaton into an equivalent DFA that has a minimum number of states.</td>
</tr>
<tr>
<td>GetRegEx()</td>
<td>Gets a regular expression for the language accepted by the NFA. The regular expression may be non-practical as it becomes long very fast.</td>
</tr>
</tbody>
</table><h2 id="operations">Operations</h2>
All basic operations with NFA-s are available:
<table>
<thead>
<tr>
<th></th>
<th></th>
<th></th>
<th></th>
<th></th>
<th></th>
</tr>
</thead>
<tbody>
<tr>
<td>Union(A1,A2)</td>
<td>Concat(A1,A2)</td>
<td>KleeneStar(A1)</td>
<td>Complement(A1)</td>
<td>Intersect(A1,A2)</td>
<td>Reverse(A1)</td>
</tr>
</tbody>
</table>
<h2 id="example">Example of creating and minimizing a FSA</h2>

```c++
#include "FiniteStateAutomation.hpp"

int main() 
{
	//Regex: a(a+b)*b + b(a+b)*a
	FiniteStateAutomation A("a(a+b)*b + b(a+b)*a");

	A.print(); //Image 1 and 2

	A.minimize();

	A.print(); // Image 3 and 4.

   	return 0;
}

```


<p>The first print:<br>
<img src="https://i.ibb.co/yfQytWy/1.png"><br>
Without the unreachable states it looks like:<br>
<img src="https://i.ibb.co/k9GPG1K/2.png" alt="image2" title="image2"><br>
And after minimizing the automation, the second print:<br>
<img src="https://i.ibb.co/M6LN38w/4.png" alt="" title="image3"><br>
It looks like this:<br>
<img src="https://i.ibb.co/qk6hTfq/3.png" alt="" title="image4"></p>
<h2 id="example">Example of getting a regex from FSA</h2>

```c++


int main() 
{	
	// FSA for ab(a+b)*
	FiniteStateAutomation A("ab(a+b)*");
		
	A.minimize(); //better to be minimized, so the regex would be simple.

	A.print();
	
	cout << A.getRegEx();

   	return 0;
}
```
Here is the FSA:


![
](https://i.ibb.co/Hzr0pmM/71001177-717157422041121-5409095346024349696-n.png "FSA to regex &#40;example 2&#41;")


And the result:


![
](https://i.ibb.co/frkg9K6/Capture.png "regex example")


**ab+(ab(a+b)\*(e+a+b))** 

**= ab + (ab(a+b)\*)**  *(since e,a and b are in (a+b)\*)*

**= ab(a+b)*** *(since ab is in ab(a+b)*\*)

<h1 id="NPDA">2. Non-deterministic Pushdown Automation</h1>

![
](https://i.postimg.cc/3rqMn6gS/PDA.png "regex example")

<p>.Non-deterministic Pushdown Automation is P = &lt;Q,Σ,G,#, s,F,δ&gt;, where<br>
Q - is a finite, non-empty set of states<br>
Σ- is the input alphabet (a finite, non-empty set of symbols).<br>
G-  finite set which is called the stack alphabet.<br>
# - initial stack symbol. <br>
s - is an initial state, an element of Q<br>
F- is the set of final states, a  subset of Q<br>
δ- is the transition function</p>

**! The empty string/symbol is $ !**

**Each rule(transition) looks like this:**

****<current state, read symbol from the tape, top of the stack, destination state, string to replace the top of stack>****

* Example 1: <0,'a', 'A', 1, "BA"> From state 0 to state 1, we push B to the stack 
* Example 2: <0,'b', 'A', 3, "$"> From state 0 to state 3, we pop A from the stack (since $ is the empty string) 

**Example for NPDA for { ww^rev | w in {a,b}* }**
```c++
// Example for Nondeterministic pushdown automata for { ww^rev | w in {a,b}* }
int main()
{
	NPDA PA(3); //3 initial states

	PA.makeFinal(2);
	PA.addTransition(0, 'a', '#', 0, "A#");
	PA.addTransition(0, 'b', '#', 0, "B#");
	PA.addTransition(0, '$', '#', 2, "$");

	PA.addTransition(0, 'a', 'A', 0, "AA");
	PA.addTransition(0, 'a', 'A', 1, "$");

	PA.addTransition(0, 'b', 'B', 0, "BB");
	PA.addTransition(0, 'b', 'B', 1, "$");

	PA.addTransition(0, 'b', 'A', 0, "BA");
	PA.addTransition(0, 'a', 'B', 0, "AB");

	PA.addTransition(1, 'a', 'A', 1, "$");
	PA.addTransition(1, 'b', 'B', 1, "$");

	PA.AddTransition(1, '$', '#', 2, "$");

	std::cout << PA.accepts("abba") << std::endl; //true
	std::cout << PA.accepts("abbb") << std::endl; //false
	std::cout << PA.accepts("aaabbbbbbaaa") << std::endl; //true
}

```
<h1 id="NPDA">3. Context-Free Grammar</h1>

![
](https://i.postimg.cc/DzDG07Vk/CFG.png "regex example")

We can input out context-free grammar (CFG) and it will simulate it using NPDA.

**Example for simulating the CFG:**

**S->aSc|B**

**B->bB|$**

```c++
int main()
{
	//Example Nondeterministic pushdown automata  from a context-free grammar
	// S->aSc | B
	// B->bB | $
	// L(S) = { a^n b^k c^n | n,k \in N}
	ContextFreeGrammar cfg;
	cfg.grammarRules.push_back("S->aSc|B");
	cfg.grammarRules.push_back("B->bB|$");

	NPDA PA2(cfg);

	std::cout << PA2.accepts("abc", true) << std::endl; //true
	std::cout << PA2.accepts("aaaaaabbbbcccccc") << std::endl; //true
	std::cout << PA2.Accepts("abcc") << std::endl; //false
}
```
