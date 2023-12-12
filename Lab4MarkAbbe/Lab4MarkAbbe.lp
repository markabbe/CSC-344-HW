% Constants for the size of the grid, the distance to maintain, and the starting X-coordinate.
#const n1=25.
#const n2=25.
#const startX=13.
#const d=6.

% Positions of people in the park.
p(4,19).
p(13,7).
p(20,4).

% Definition of the grid.
grid(X,Y) :- X=0..n1, Y=0..n2.

% Determine if a position is unsafe due to another person's proximity.
unsafe(X,Y) :-
    grid(X,Y),
    p(PX,PY),
    DX = X - PX, DY = Y - PY,
    DX*DX + DY*DY < d*d.

% Determine if a position is safe (no person within the 6-foot distance).
safe(X,Y) :- grid(X,Y), not unsafe(X,Y).

% Movement rules: moving north, east, or west on the grid if the next position is safe.
move(X,Y,X,Y+1) :- Y < n2, safe(X,Y+1).
move(X,Y,X+1,Y) :- X < n1, safe(X+1,Y).
move(X,Y,X-1,Y) :- X > 0, safe(X-1,Y).

% Starting position rule, ensuring the first step is safe.
path(startX,0,startX,1) :- not unsafe(startX,1).

% Rules for building a path, ensuring exactly one move is made from each position.
1 { path(X,Y,X1,Y1) : move(X,Y,X1,Y1) } 1 :- path(_,_,X,Y), grid(X,Y).
path(startX,0,startX,1).

% Goal condition to reach the north edge of the park.
goal(X,n2) :- path(_,_,X,n2).

% Ensure the path is continuous, does not backtrack, and reaches the goal.
:- path(X,Y,_,_), not 1 { path(X,Y,X1,Y1) : move(X,Y,X1,Y1) }, grid(X,Y).
:- not goal(_,n2).

% Output directive to show the solution path.
#show path/4.