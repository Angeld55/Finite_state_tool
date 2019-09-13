---


---

<h1 id="finite-state-automation">Finite State Automation</h1>
<p>Finite state automation is A = &lt;Q,Σ,s,F,δ&gt;, where<br>
Q - is a finite, non-empty set of states<br>
Σ- is the input alphabet (a finite, non-empty set of symbols).<br>
s - is an initial state, an element of Q<br>
F- is the set of final states, a  subset of Q<br>
δ- is the state-transition function</p>
<h1 id="creation">Creation</h1>
<p>Diffrent ways of creating an automation</p>
<pre class=" language-c"><code class="prism ++ language-c"><span class="token keyword">int</span> <span class="token function">main</span><span class="token punctuation">(</span><span class="token punctuation">)</span> 
<span class="token punctuation">{</span>
	
	<span class="token comment">//With  regular expression (Have to use many brackets for now) *fix later</span>
	FiniteStateAutomation <span class="token function">A</span><span class="token punctuation">(</span><span class="token string">"((a).(((a)+(b)))*)"</span><span class="token punctuation">)</span><span class="token punctuation">;</span>

	
	FiniteStateAutomation A2<span class="token punctuation">;</span><span class="token comment">//Automation with one state (for now)</span>
	<span class="token comment">//Don't forget to put the letters for the alphabet. In the regular expression way of creating an automation, it does it automatically.</span>
	A2<span class="token punctuation">.</span><span class="token function">AddLetterToAlphabet</span><span class="token punctuation">(</span><span class="token string">'a'</span><span class="token punctuation">)</span><span class="token punctuation">;</span>
	A2<span class="token punctuation">.</span><span class="token function">AddLetterToAlphabet</span><span class="token punctuation">(</span><span class="token string">'b'</span><span class="token punctuation">)</span><span class="token punctuation">;</span>

	A2<span class="token punctuation">.</span><span class="token function">AddState</span><span class="token punctuation">(</span><span class="token punctuation">)</span><span class="token punctuation">;</span>
	A2<span class="token punctuation">.</span><span class="token function">AddTransition</span><span class="token punctuation">(</span><span class="token number">0</span><span class="token punctuation">,</span> <span class="token number">1</span><span class="token punctuation">,</span> <span class="token string">'a'</span><span class="token punctuation">)</span><span class="token punctuation">;</span>
	A2<span class="token punctuation">.</span><span class="token function">AddTransition</span><span class="token punctuation">(</span><span class="token number">1</span><span class="token punctuation">,</span> <span class="token number">1</span><span class="token punctuation">,</span> <span class="token string">'a'</span><span class="token punctuation">)</span><span class="token punctuation">;</span>
	A2<span class="token punctuation">.</span><span class="token function">AddTransition</span><span class="token punctuation">(</span><span class="token number">1</span><span class="token punctuation">,</span> <span class="token number">1</span><span class="token punctuation">,</span> <span class="token string">'b'</span><span class="token punctuation">)</span><span class="token punctuation">;</span>


   	<span class="token keyword">return</span> <span class="token number">0</span><span class="token punctuation">;</span>
<span class="token punctuation">}</span>


</code></pre>
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
<td>Adds a new state. Return the index of the new state.</td>
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
<td>Recognize(word)</td>
<td>Return true if the word is accepted by the DFA/NFA.</td>
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
<td>MakeComplete()</td>
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

<table>
<thead>
<tr>
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
<td>KleeneStar(A1,A2)</td>
<td>Complement(A1)</td>
<td>Intersect(A1,A2)</td>
</tr>
</tbody>
</table><h2 id="example">Example</h2>
<pre class=" language-c"><code class="prism ++ language-c"><span class="token macro property">#<span class="token directive keyword">include</span> <span class="token string">"FiniteStateAutomation.hpp"</span></span>

<span class="token keyword">int</span> <span class="token function">main</span><span class="token punctuation">(</span><span class="token punctuation">)</span> 
<span class="token punctuation">{</span>
	
	FiniteStateAutomation <span class="token function">A</span><span class="token punctuation">(</span><span class="token string">"((((a).(((a)+(b)))*).(b))+(((b).(((a)+(b)))*).(a)))"</span><span class="token punctuation">)</span><span class="token punctuation">;</span>
	<span class="token comment">// a(a+b)*b + b(a+b)a (starts and ends with diffrent letters</span>
	
	A<span class="token punctuation">.</span><span class="token function">Print</span><span class="token punctuation">(</span><span class="token punctuation">)</span><span class="token punctuation">;</span> <span class="token comment">//Image 1 and 2</span>

	A<span class="token punctuation">.</span><span class="token function">Minimize</span><span class="token punctuation">(</span><span class="token punctuation">)</span><span class="token punctuation">;</span>

	A<span class="token punctuation">.</span><span class="token function">Print</span><span class="token punctuation">(</span><span class="token punctuation">)</span><span class="token punctuation">;</span> <span class="token comment">// Image 3 and 4.</span>

   	<span class="token keyword">return</span> <span class="token number">0</span><span class="token punctuation">;</span>
<span class="token punctuation">}</span>

</code></pre>
<p>The first print:<br>
<img src="https://lh3.googleusercontent.com/kn6hdNn2ZmkjFB9BsS0f5W2FcM23ewhZeblTIVUculm-oMcd1O4EPuuMCIYWl1ZL3tKfcVGrCA" alt="image1" title="image 1"><br>
After removing the unreachable states it looks like:<br>
<img src="https://lh3.googleusercontent.com/Wt8ptsgMGEi69QHjXjRiaKr_45mPWnQFspO39iKbA-5KCWt1AkF2jG8ovOVELPqYGcS7PBeBEw" alt="image2" title="image2"><br>
And after minimizing the automation, the second print:<br>
<img src="https://lh3.googleusercontent.com/iGMNgwBvphgeFmv2rNXrr0yMqtkYyLlJ-rQhy-pHdRRAGgxKRWh3_e2KSCklqwAkDGCCArbrqA" alt="" title="image3"><br>
It looks like this:<br>
<img src="https://lh3.googleusercontent.com/67qZQH5u6d3dNHdvcXpm-0pZYoIkuy2Taw_IgKTZR7NaTRYeCovlGQA4zSzEBZxN_EP8qvX7dg" alt="" title="image4"></p>

