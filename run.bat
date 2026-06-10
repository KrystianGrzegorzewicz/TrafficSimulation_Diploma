@echo off

set EXP=SP10_AV00_J5_BIA05_SHA05_MAX30

for /L %%i in (1,1,10) do (

	start "" /B x64/Debug/TrafficSimulation.exe ^
		--nogui ^
		--spawn 1.0 ^
		--av 0.0 ^
		--junction 5 ^
		--seed %%i ^
		--run %%i ^
		--max-finished 30 ^
		--experiment %EXP% 
)