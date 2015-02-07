Guidelines for Contributing to OpenSim-Core
===========================================

We encourage everyone to contribute to the OpenSim-Core repository. This could be making a pull request, as described below, or letting us know about a bug or issue. We've set up the following guidelines for making contributions through a pull request. The purpose of our contribution policy is to ensure that all code in OpenSim-Core has undergone real scrutiny, thereby reducing the likelihood of errors. 

Even great programmers benefit from additional eyeballs on their code to catch bugs, avoid duplication and inefficiency, watch for standards violations, and to check that the code is as clear to others as it is to its author. We appreciate contributions and our development team is collaborative and constructive -- don't be shy!

Thank you for contributing!

  - [Making a Pull Request](#making-a-pull-request)
  - [Checking for Memory Leaks through GitHub](#checking-for-memory-leaks-through-github)
  - [Contributors](#contributors)
  - [Coding Standards](#coding-standards)
    - [Header Guards](#header-guards)
    - [Creating New OpenSim Objects](#creating-new-opensim-objects)
    - [Assignment Operators in C++](#assignment-operators-in-c)
    - [Documenting your Code](#documenting-your-code)
    - [Each line of text should be at most 80 characters long](#each-line-of-text-should-be-at-most-80-characters-long)
    - [Replace Tabs with Four Spaces](#replace-tabs-with-four-spaces)
    - [Renaming Classes in the OpenSim API](#renaming-classes-in-the-opensim-api)
    - [Naming Conventions](#naming-conventions)
    - [Other C++ Coding Style Suggestions](#other-c-coding-style-suggestions)
      - [Throw and return are not functions](#throw-and-return-are-not-functions)
      - [Always use pre-increment and pre-decrement operators when you have a choice](#always-use-pre-increment-and-pre-decrement-operators-when-you-have-a-choice)
      - [Place pointer and reference symbols with the type](#place-pointer-and-reference-symbols-with-the-type)
    - [Removing Methods](#removing-methods)

Making a Pull Request
---------------------
1. To help the people who review your pull request, you must include a detailed description of what changes or additions the pull request includes and what you have done to test and verify the changes. If there is an existing issue or set of issues that documents the problem the pull request is solving, you can reference it. But, please make sure detailed information about the commits is easily available.

2. Make sure that your request conforms to our [coding standards](#coding-standards).

3. Make sure that tests pass on your local machine before making a pull request. The [README.md](https://github.com/opensim-org/opensim-core) mentions how to run the tests.

4. Typo fixes can be merged by any member of the Dev Team.

5. Updates to comments. Doxygen, compiler compatibility, or CMake files must be reviewed by at least one member of the Dev Team before being merged. The original author or the reviewer(s) may merge the pull request.

6. Any other changes to the code require review by at least two members of the Dev Team. If the pull request involves adding a new class or performing a major object/algorithm refactor, one of these reviewers must be an Owner. The Owners and Dev Team are Teams within the opensim-org GitHub organization. The original author may NOT merge the pull request. 

A few additional practices will help streamline the code review process. Please use tags (i.e. @user_name) and quoting to help keep the discussion organized. Please also call for a meeting or Skype call when discussions start to stagnate. In addition, we recommend getting input on your interface design before implementing a major new component or other change.


Checking for Memory Leaks through GitHub
----------------------------------------
If you want to check memory leaks, you can have Travis-CI run valgrind on the test cases. Just put `[ci valgrind]` in your commit message.


Contributors
------------
Here is a partial list of contributors; please let us know if you know of a missing name.

- Frank C. Anderson
- Allison S. Arnold
- Edith Arnold
- Cindy Au
- Scott L. Delp
- Chris Dembia
- Matt S. DeMers
- Tim Dorn
- James Dunne
- Peter Eastman
- Dominic Farris
- Lorenzo Flores
- Melanie Fox
- Brian Garner
- Saryn R. Goldberg
- Eran Guendelman
- Ayman Habib
- Samuel R. Hamner
- Jennifer L. Hicks
- Katherine R. S. Holzbaur
- Daniel Jacobs
- Chand T. John
- Cassidy Kelly
- Marjolein van der Krogt
- Joy Ku
- Glen Lichtwark
- Jingjing Liiu
- May Q. Liu
- Peter Loan
- Katie Lund
- Jack Middleton
- Matthew Millard
- Paul C. Mitiguy
- Carmichael Ong
- Soha Pouya
- Apoorva Rajagopal
- Jeffrey A. Reinbolt
- Ajay Seth
- Amy Silder
- Andreas Scholz
- Michael A. Sherman
- Ian Stavness
- Katherine Steele
- Justin Si
- Darryl G. Thelen
- Tom Uchida
- Jack Wang
- Kevin Xu
- Jenny Yong

# Coding Standards

## Header Guards

Header guards are preprocessor defines that surround every header file to prevent it from being included multiple times. OpenSim header guards should be written like this:

```cpp
#ifndef OPENSIM_PROPERTY_TABLE_H_ // <-- yes, do it this way!
#define OPENSIM_PROPERTY_TABLE_H_
 // ... stuff ...
#endif // OPENSIM_PROPERTY_TABLE_H_
```
This matches the scheme used in Simbody and avoids all of the following problems. (Trailing underscore, but not leading, is allowed.)

They should not look like this:

```cpp
#ifndef _PropertyTable_h_ // <-- don't do it this way!
#define _PropertyTable_h_
 // ... stuff ...
#endif //_PropertyTable_h_
```

There are two problems with these header guards:

1. They can interfere with user code, and
2. They violate the C++ standard.

OpenSim uses many very common class names, like “Object” and “Array” and probably “PropertyTable”. These are likely to appear in other code libraries as well, so anyone who combines OpenSim API with other libraries or their own code may have conflicts. A simple rule to avoid conflicts is for the header guards to be unique symbols, easily achieved by including the product name in them (that is, they should contain “OPENSIM”).

As a reminder, the C++ standard prohibits user code from using identifiers that contain double underscore (“__”) or begin with an underscore followed by a capital letter (“_P”). Those symbols are reserved for the compiler and the std:: library. Use of symbols like that means it is subject to conflict with the compiler, either now or in future releases or new platforms. Hopefully those conflicts would cause compiler errors, but that is not guaranteed.

One other minor point is that preprocessor macros should typically have very ugly names with LOTS_OF_CAPS to make it obvious that they are not ordinary identifiers, and to avoid conflicts with NiceUpperCamelCaseIdentifiers used for class names.

## Creating New OpenSim Objects

Every OpenSim object class now automatically defines a typedef “Super” that refers to the immediate parent (“superclass”) of that class. If you have to delegate something to your parent, use “Super” rather than explicitly listing the parent class, because if that changes due to future refactoring your code will still compile but be wrong! This is not a hypothetical problem -- I found bugs of that sort sprinkled around OpenSim.

Example:

In MyNewComponent.h:

```cpp
class MyNewComponent : public SomeIntermediateClassDerivedFromObject {
 ...
 void extendBaseClassMethod() override;
}
```

In MyNewComponent.cpp:

```cpp
void MyNewComponent::extendBaseClassMethod() {
 Super::extendBaseClassMethod(); // invoke the parent’s method
 //NOT: SomeIntermediateClassDerivedFromObject::extendBaseClassMethod()
  
 // now do the local stuff
} 
```

Now if someone changes the class structure later so that you component’s parent changes in the header file, the code in the .cpp file (which will have been long forgotten) will automatically change its behavior.

##Assignment Operators in C++

You should let the compiler automatically generate the copy constructor and copy assignment operator for your classes whenever possible. But sometimes you have to write one. Here is the basic template for copy assignment:


```cpp
MyClass& operator=(const MyClass& source) {
 if (&source != this) {
   // copy stuff from source to this
 }
 return *this;
} 
```

You run into problems if copy assignment operators are missing lines 2 and 4. Since the “copy stuff” part often begins by deleting the contents of “this”, a self assignment like a=a will fail without those lines; that is always supposed to work (and does for all the built-in and std library types). Of course no one intentionally does that kind of assignment, but they occur anyway in general code since you don’t always know where the source comes from.

If the “copy stuff” part consists only of assignments that work for self assignment, then you can get away without the test, but unless you’ve thought it through carefully you should just get in the habit of putting in the test.

## Documenting your Code

Doxygen only looks in your .h files. It does not generate documentation from cpp files. Thus comments in .cpp files don't need to follow doxygen formatting, and in fact they should not because it is confusing and makes it look like there is API documentation when there isn't. You should mostly use "//"-style comments in .cpp files, and be sure you are addressing your comments to the right audience -- no doxygen reader will ever see them.

Read more about doxygen on this page: Guide to Building Doxygen

## Each line of text should be at most 80 characters

It is common to display multiple code windows side by side. For the code to display nicely,
it is necessary to choose a limit on the line length.

## Replace Tabs with Four Spaces

Please be sure that your IDE (code editor) is set to replace tabs with four spaces. You should never allow tab characters to get into your code. They will look great to you, but most other people will see your code as randomly formatted.

If you use Visual Studio, goto Tools:Options:Text Editor:C/C++:Tabs, set tab size=indent size=4, and check the "Insert spaces" button.

## Renaming Classes in the OpenSim API

Sometimes it makes sense to change the name of a class in OpenSim because the name is confusing or doesn't reflect the desired function. This seemingly innocent, and usually desirable refactoring, has some side-effects that API developers should be aware of so that changes do not break working functionality.


**Deserialization** : The code that reads objects from XML files keys on the String representing class name to create corresponding objects (e.g. "PinJoint" class shows in XML as <PinJoint>). If you change the name of PinJoint (e.g. to MyPinJoint) you need to make sure old models that have the tag <PinJoint> still work. Normally this is captured by test cases. If you decide to make the change, you'll have to edit the file "RegisterTypes_osimSimulation.cpp" and add the line Object::renameType("PinJoint", "MyPinJoint"), so that the deserialization code knows how to handle the XML tag.

**Swig wrapping and GUI** : Most API users don't build the GUI, however they should continue to build the JavaWrapping to make sure changes on the C++ side do not cause serious problems downstream to either the GUI or scripts that we'll be distributing that utilize the Java wrapping. The mechanics for this procedure are as follows:
- Turn on JavaWrapping in CMake.  You have to have SWIG and Java installed.
- Build JavaWrap project to run SWIG (see README.md for obtaining SWIG).
- Run test case testContext which ends up simulating a few GUI calls.

If a class is not included in the wrapping interface file [OpenSim/Java/swig/javaWrapOpenSim.i](https://github.com/opensim-org/opensim-core/blob/master/OpenSim/Wrapping/Java/swig/javaWrapOpenSim.i) then the class is likely not used by the GUI and so is safe to change, otherwise please consult with GUI developers first before renaming.

## Naming Conventions
Please follow the convention that property names use “lower_case_with_underscores” as their names, while object types use “CamelCaseUpAndDownWithoutUnderscores”. That ensures no conflicts with XML tag names and makes it easy to tell a property name from an object name.

## Other C++ Coding Style Suggestions

### Throw and return are not functions

In C++ “throw” and “return” are not functions. It is misleading to enclose their arguments in parentheses. That is, you should write “return x;” not “return(x);”. A parenthesized expression is not treated the same as a function argument list. For example f(a,b) and return(a,b) mean different things (the former is a 2-argument function call; the latter is an invocation of the rarely-used “comma operator”).

### Always use pre-increment and pre-decrement operators when you have a choice

Both pre-increment i and post-increment i are available. When you don’t look at the result, they are logically equivalent. For simple types they are physically equivalent too. But for complicated types (like iterators), the pre-increment is much cheaper computationally, because it doesn’t require separate storage for saving the previous result. Therefore you should get in the habit of using pre-increment in all your loops:

 
```cpp
/*YES*/ for (int i; i < limit; ++i); 
 
/*NO*/ for (int i; i < limit; i++); 
```

This will prevent you from using the wrong operator in the expensive cases, which are not always obvious.

Of course in cases where you actually need the pre- or post-value for something, you should use the appropriate operator. 

### Place pointer and reference symbols with the type

References and pointers create new types. That is “T”, “T*”, and “T&” are three distinct types. You can tell because you can make typedefs like this:

```cpp
typedef T  SameAsT; 
 
typedef T* PointerToT;
 
typedef T& ReferenceToT;
 
// and then declare
 
SameAsT      t1,      t2;      // both are type T
 
PointerToT   tptr1,   tptr2;   // both are type T*
 
ReferenceToT tref1=a, tref2=b; // both are type T&
```

Therefore you should place the “*” and “&” next to the type, not the variable, because logically they are part of the type. Unfortunately, the C language had a bug in its syntax which has been inherited by C++. A line like “char* a,b” is treated like “char* a; char b;” rather than “char* a; char* b;”, but if I write “typedef char* CharPtr;” then “CharPtr a,b” declares both to be pointers. There is no perfect solution because the language is broken. However, there is no problem in argument lists (since each variable has to have its own type). So I recommend that you simply avoid the misleading multiple-declaration form when using pointers or references. Just use separate declarations or a typedef. Then always put the “*” and “&” with the type where they belong. So argument lists should look like this:

```cpp
/*YES*/ f(int I, string& name, char* something);
 
/*NO*/ f(int I, string &name, char *something);
```

## Removing Methods

When cleaning up classes and removing methods, if you decide to remove a method then it's necessary to remove both the prototype from the header and the implementation from the cpp file (if any). While C++ doesn't complain, leaving the prototype in the header file with no implementation anywhere causes problems for wrapping. Swig runs only on the headers and has no way of knowing if there's an implementation or not. Since the methods end up being exported, they then have to be resolved at compile time of the osimJavaJNI project. 
