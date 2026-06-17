@echo off

for /L %%i in (3,1,5) do (
	for /L %%j in (1,1,1) do (
		for %%k in (0.25 0.50 0.75) do (
			for %%l in (0.25 0.50 0.75) do (
				for %%m in (0.4 0.7 1.0) do (
					start "" /B x64/Debug/TrafficSimulation.exe ^
						--nogui ^
						--spawn %%m ^
						--av 0.0 ^
						--junction %%i ^
						--bias %%k ^
						--sharpness %%l ^
						--seed %%j ^
						--run %%j ^
						--max-finished 30 ^
						--max-time 500
				)
			)
		)
	)
		
)