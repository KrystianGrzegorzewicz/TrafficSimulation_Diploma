@echo off

set EXP=SP05_AV00_J3_BIA05_SHA05_MAX20

for /L %%i in (1,1,10) do (

	start "" /B x64/Debug/TrafficSimulation.exe ^
		--nogui ^
		--spawn 0.5 ^
		--av 0.0 ^
		--junction 3 ^
		--seed %%i ^
		--run %%i ^
		--max-finished 20 ^
		--experiment %EXP% 
)