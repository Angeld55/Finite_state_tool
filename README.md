<h1 id="finite-state-automation">Finite State Automation</h1>

![enter image description here](https://lh3.googleusercontent.com/31mG7PgutczmaqoLakuiaFhf7M3iC6sVzZihcKZZee-H7oPbqj9T2nHjv8sgbFHPpkRmeQlNfDpXdOO8vIzlP0c84UO-iVnHmBfJvHrxWoqIGLkesQ3W40LDkGUFw8TgjbfAwLuJ8H3STAc3j4gTRwVWrDMSDC9ZpQuCR9RybMv-EyUxfHu11JrnGYFXfy127MnMylvm2lScaqTj9obWEXw5-ajxtc96v41qM-HosIbNCyRK0hwxOmUO8srxmP5XDEf74ndScnzZkr6jrdzU6t6Aokc-yqcwI1k3H2ogIAJ2aWX1gW_5NgH8p7Qexi1EWp94ZRfnhoUm8JPxFIT_wWJYMWU0WSIlP69ECePuOU5uX5eDRMbdz2OqQ0zO2pPQ_GfjeA0z8sHl_q4vWFb-HU3DCh7bzC8C8d6oy7iwgnQBPu38TA2D47wgERAkiRBx42HvTealJP7aYiZcCMFqY0YzlCR66Qh6SUzU5gOFWRbINrW2tgX72sb41lBDrsXtZmpLmuXqnjqgTn0n83NHDaN2gJGkEY_ybSa5s0cBcNpzOEpiZEApIoxC57O1UKsJbpwwbCG4KkBVe6mIQNMlvnAamhOvyOKRyVaog7qV7z_3liS8YpFekcidIUvQB-v1027OHr3Qegr29uOuene_gyJnpok8r9dEdyOR0MnUTbwsANhB_Qq55g=w275-h157-no?authuser=0 "example of FSA")


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
<img src="https://lh3.googleusercontent.com/lG93MZhBqqTVlHhKVHt-pdQCabEZCXfkmLcxvNScIYLXeBymuJ9ml7kKDWInnPrKd82uP17bb7B-yuZEJVj3m1W_HSFACnzNmIo1w5eSjBooALJK9mOpov4m0CD-ntosTdYTFJPipf1JlGnHJHQtKW7bQc2FPUHxPNK-V8SrOipmY-X7sRl0qtRAtTRx7DPQ6R05lRzR0WsjZobL1K3YqwprKqyFXPqP0Tpp-85VuD6k6SuFLo-xTSbp8Y2AnQrMn-nc-sj9zHl_YTtXpd5g9PqJ2v8RgtPbpjExRdXSmID_OgZWGheYsryrrnFu6e5R4gXvWuSC9WUFkAmlbVXWpvxNGLULqxeNwoFuyk90np8xDPEfr0RivGpkaL2w-_0tShe4dHTZSwDTHfwrcR2P4DHqXNYdxVlah7DKRzlPDUS_JXBHHcQlhw7p1vpS5zT1Gs4lpF9MNihKMJhPdIT4fClLwHxjw9cNluhON2CqaoxCUG7TSw8U9iict9vS3ZVN2rcYPAd0og1lBtCUFp28GFC-YidtLDXgHk9TA_T0o9YMLUA0f8aazzRpFB7hS4xB6o2uHjgpTDo-NnLoUY6x_DYDwPZd_j97kCtYKaJRxKEXKs8r2RDd0Hw5mPWqASpsGch4Igr6Ug9a3Yw2dlKD1s3NeHd0rvEn3ZZaZS5Kr7UQfJXjNRfgJg=w853-h384-no?authuser=0"><br>
Without the unreachable states it looks like:<br>
<img src="https://lh3.googleusercontent.com/bpfjBbhWrPy-b5VN7HkRji31olciqC4mDvdU6UdIsY2fxAKrFN6OwO7Pg57k71RRAt3oQHOTa7knYcee03MGUb3HAsOyhVyoX7X0GWUVqoeBQDP72bpJB-cLJUIs0pjPSeer4ZxUCL5hTnfwurmDJiG3pMB0WNGruf5Un9c0oApXrgAXwsVZGwW_rGRLzHXXLuS9Echs7ZO6-d6RvUe3Zc8BtjfEUmaW7ujMXWv4gfQmeBFLrzZQOFFoggxXQ2mzlhQ-ZDfK3CCAZ8i45ZfjlWiqsVLExP_gCUcQ5ng8RVTzwiw5R5nXp7UgyGQQ9lawvAccAziPalsE9S_DZFOX7xpR2A8gYTxTtJufOh9NMWhKUvzBEqJcYtFwdJ-7JlgO6tdxMPJJmGwrFeuEL4yt4k2yrCzxZIoemlBwnYQG78_5rr_5iec8EZDqKnd3qJkYU1in6sTT_yQPTkMPjbgy5Pemq2rfPA-zuzDgt3_nXRW9bX5BNt3_j5onPOmEijpdBKJepJ03JVoAmBvbz3xCfMx58bM-0i069Uq6sEao-9Zsfu35VTmGU5HorHuMtaJIOMtWgAryrkg4smpKVUpUkyeb1A3ex6KkuUGcFF_cflSPaspkOsoiaQatpRnjX6L7UD5zKGhEGf-qm4C92VDv93d1rd9kPjhwlqbFMT6hjajvDePfo_Y5dg=w686-h564-no?authuser=0" alt="image2" title="image2"><br>
And after minimizing the automation, the second print:<br>
<img src="https://lh3.googleusercontent.com/HYE-R6oyRCrhXvatk_jawG_Y8wvO5LkGkEi9WJTOQ9ozZN0snBH5m1izefEK8n9ZnAiuWqGRvWWcVVmeUgsmZxwlvYR6Eh_j8xtLgNISSV2JIi_tFa6SahS-75Pctz7wQeXJW-LSFS5SwIPc583vd68-oAEs74DhLrwn09F3gdO3YQj-neSoF5uAgZEXIvWtX9wSxxbkVhf87sYEbAusiDS2zVZW0XStlKuOnH83vS0bkkFeyGp_WjZLE1wWygAkYH9Yqbb5tZEWTHOQtKiXIwdeQ_4PBRQuvGsudoxgIz__uZeFiMnivrTOifK7B1Q6Xhl63wqYibHHXM_u-9AdqaOTUWXr70QgURl5FZga6XWLws6H3vqQspDw68PdFbCheDLtvPZlWyLUcpwfkxBT8UcfaRHX4168c9_wWLhaZXtlt-WafaBuwmHkhp6cxMXlI4JOCLZ4EL_zO5Zz09LAIKS2xGYR9tOs838bHCPLqTbG49C63oEXT1JsniYZX8EPLtKGM4c5wZQTKlh4BYC1LgSjlFsV72jpfYT_k4JCCJ8jGxUpbi1M_B9Pz07hiOz_alUXbsgZnZGljp1ZcrDe6M8zVrWtsL4FHxDxWuYvZWDnpbUAkRKgv5bvdHhFso-n0tEQKln9z3JbiXxS0JPtGxRhNb86TOnyLbhijVmWR6nvQe7nuZ1vUg=w652-h265-no?authuser=0" alt="" title="image3"><br>
It looks like this:<br>
<img src="https://lh3.googleusercontent.com/ldf9gIhtg_fsotDwp6XsV6w-F-q5ye0RRu8EWYxOPa_tdkXMIotRcVT388tAzuOPN2KyFVHMDXfT4A-eYGBNSSgddhvpjP_eB-qxtDNcsLGx8fOYq2S1eVhcSAcJPfs7w7r-HZprQfe7LdyAMT07Do32PjUFE7gQ6jE5_8YJ39Lb3-iR7Ct9RyW8y38WLFn-EKw6SKnfeJM0CIaPkSoek8lbRI5cu5JBnSZoJWJqF29MJ6ETwNVX7oJVL8BlMXwgK9dyJx7_av262g9MGPH-SRseENPKo2kS1S1QA1qoesHYkE22gT6R_yJsoR--RtAJGXveSQF2WWhE-bd4ZC62GhPijBYGhnbbrDT7Fcvha1Mjn9MNIBe-KKM8Y-R9-lBaTGef8Z2iVILIUQYMz7RaJa_GRL-eab3zXn1VMGCFKGt3Ni0GaMx-JIkPiNvaEvoyldNx6o3-c2b24lO8mUtUge4PtSKKBzo0PuXHlS5arDV3V0NsUiL-OFOWJ-bKmfVMJfJC7d09cYZXlr3uKDKe5OUywLpD_T4dzDxAHN4UecmVx25ioZ0uixBloSC7eVlHsYIkaGEkexK1-Ncwst1SjJGS1nj0PkcZ5uPkJfAT7r94oMgqm0nU6I4Z2oEwRNMmDu5dmf-UycuoPy_M9ubmrpFQWzTIQX1HtZcEzU49ALx9jUIdAX42oQ=w528-h314-no?authuser=0" alt="" title="image4"></p>
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
](https://lh3.googleusercontent.com/9xZ9GA_NP6yAAjPER-vp19TbapbWFZYfsm3963ScUGIUoynVMkVT8eJaA-AEUKEb_Wy8ncLZacpSC_UOP3ukw5qFesYJNvUdUoYRhnFwHD7sP-QB-gBygUwgnj1TFjiQigmkmr8YH0Vm2ow5eGOTpK-hciFdfs2t5HTLy6wEkr5mOtCGxJP2mtPApyatOWatuc7Cqa7BCMJwmmsorjmL3-KaPMoFElb0eUdvhxiYyqbcP7r1TPm2jRL7ckNCniLVeXfejXxqkiWQsQto_ww4Pdz4EhGCbGMijhhM0WaKhbq2dUd9uLlsF8nQ9rYMMY7kYyOIhGnv0wnGhgV0zXe1em6xUlc3Vo78saQf1ELOBCMgQ_jrItFGY0q6jeMmQ2dMWelXfRkOSf5nYTGkHn5xepcq9crrhaSUQifcnsEPhm8j8DZ9CKP_BrcFxgL-psyaZ2X-6gHtYxh33KQh-A8wqFVag9j2E3nk78femDV2xstmUl3Q4uZ55LMAEYdbsHq9B-FxBanW8ehrHT9mvjtHlBzmS5aZkA6EGGvJoHx4XzhFP3rN9sFMYQBvzT_OhbzaJb5MlE6LEQkiF0c5bnRUqiOQqQ6WGhQDOe-HiYeQsFk2_YSzv4OD6f3ZQhLxwl9Ja2HYP9DuoGp8KHfSBJMH1id2nmIyLDItfjGddB-6NezQazqNWnPGgQ=w399-h205-no?authuser=0 "FSA to regex &#40;example 2&#41;")


And the result:


![
](https://lh3.googleusercontent.com/0vCKOAtWv5YXV_QIVrKOzc_DA4eROs49MTgHVNWw5EMfGdlNljxT76TzP8YMDNiiAFhabk1zam1ywObKlfdu-iUfFRoOyGsCIaw-VOLp4JWNPwJUiaPpm7G9cQUc41YcAooB4gTL-vU9z8vtXyi9Yjes1Wsj6tBoJNXR2qZkfRb9MTb-R9jegWmvlehhZkYHMxNbNjAL5rp8XB-25BpbBbGVV4nOImB-qEvazq1vIe-UuE_uC9C07vkdzoRUpG5vgww_XcYYfr6ckkLl6otKVTYNqi6kOv3Gft3iOmqtd3hILyjPTjwjMPIFVe-R_Gt88D59NRpJc-zBIX28u3Uwo07WnHr12zewbXwOFE1w3_H1qQ6tSrFxbD6I-DNO5kR5qq6nmOS3B51k5UeKmbEtQCwge-R8jjpuEwZafkOHao5DsSzEXlI0n5knoad2vh3FrKLPKHKpWBLbb_Had_gAishqUSIZzWZ_XfegEz3yW2yXUHHSAXBso4INDxl_eRXtMV3E-6d3VAuoeHL1-0fXuVcXy-k3Btjdzgxz959XnGXQzg-3EgYvi42XY2gPP5VfuNgbfmmrH2xSNjUxHRR4ijeWrzYfskhBTdr_JVfNPHDFFhbYBEcREeEq7TrAjjssuZs1obIAHcQP9PRSshRQhIX1bhvBT1mRfby5AGX6V4CDITib-8yLkw=w394-h57-no?authuser=0 "regex example")


**ab+(ab(a+b)\*(e+a+b))** 

**= ab + (ab(a+b)\*)**  *(since e,a and b are in (a+b)\*)*

**= ab(a+b)*** *(since ab is in ab(a+b)*\*)
