@echo off

set EXP=AV50_TEST

for /L %%i in (1,1,10) do (

	start "" /B x64/Debug/TrafficSimulation.exe ^
		--nogui ^
		--spawn 1.0 ^
		--av 0.5 ^
		--junction 5 ^
		--seed %%i ^
		--run %%i ^
		--max-finished 1000 ^
		--experiment %EXP%

)