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
	FiniteStateAutomation A("((a).(((a)+(b)))*)");


	FiniteStateAutomation A2;//Automation with one state (for now)
	//Don't forget to put the letters for the alphabet. In the regular expression way of creating an automation, it does it automatically.
	A2.AddLetterToAlphabet('a');
	A2.AddLetterToAlphabet('b');

	A2.AddState();
	A2.AddTransition(0, 1, 'a');
	A2.AddTransition(1, 1, 'a');
	A2.AddTransition(1, 1, 'b');


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
<td>Accept(word)</td>
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
	FiniteStateAutomation A("((((a).(((a)+(b)))*).(b))+(((b).(((a)+(b)))*).(a)))");

	A.Print(); //Image 1 and 2

	A.Minimize();

	A.Print(); // Image 3 and 4.

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
#include "FiniteStateAutomation.hpp"

#include "FiniteStateAutomation.hpp"

int main() 
{	
	// FSA for ab(a+b)*
	FiniteStateAutomation A("(((a).(b)).(((a)+(b)))*)");
		
	A.Minimize(); //better to be minimized, so the regex would be simple.

	A.Print();
	
	cout << A.GetRegEx();

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
](https://i.ibb.co/frkg9K6/Capture.png "regex example")

<p>.Non-deterministic Pushdown Automation is P = &lt;Q,Σ,G,#, s,F,δ&gt;, where<br>
Q - is a finite, non-empty set of states<br>
Σ- is the input alphabet (a finite, non-empty set of symbols).<br>
G-  finite set which is called the stack alphabet.<br>
# - initial stack symbol. <br>
s - is an initial state, an element of Q<br>
F- is the set of final states, a  subset of Q<br>
δ- is the transition function</p>

