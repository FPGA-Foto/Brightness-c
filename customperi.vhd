-- HSB_PERIP.vhd

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.ALL;
--USE ieee.std_logic_arith.all;
USE ieee.math_real.all;

library std;
use std.standard.all;
use ieee.fixed_pkg.all;
-----------------------

entity HSB is
	port (
		clk			: in  std_logic;
		imageWidth	: in std_logic_vector(8 downto 0);
		imageHeight	: in std_logic_vector(8 downto 0);
		x				: in std_logic_vector(8 downto 0);
		y				: in std_logic_vector(8 downto 0)
	);
end entity HSB;

architecture HSB_PERIP of HSB is
	signal STATUS	 	: std_logic_vector(7 downto 0) 	:= "00000000";
	SIGNAL NXTSTATUS 	: std_logic_vector(7 downto 0) 	:= "00000000";
	
	-- return this
	signal newPosition		: std_logic_vector(8 downto 0)	:= "000000000";
	--signal output				: sfixed(7 downto -8)	:= "0000000000000000";
	--signal fixedX				: sfixed(7 downto -8)	:= "0000000000000000";	
	--signal fixedY				: sfixed(7 downto -8)	:= "0000000000000000";
	--signal fixedImageWidth	: sfixed(7 downto -8)	:= "0000000000000000";	
	--signal fixedImageHeight	: sfixed(7 downto -8)	:= "0000000000000000";	
	--signal yFactor				: sfixed(7 downto -8)	:= "0000000000000000";
	--signal yOffset				: sfixed(7 downto -8)	:= "0000000000000000";
	--signal normalizedY		: sfixed(7 downto -8)	:= "0000000000000000";
	--signal normalizedY2		: sfixed(7 downto -8)	:= "0000000000000000";
begin

	process(clk)
	begin
		
		-- When the peripheral is reset
		if (rising_edge(clk)) then
			STATUS <= NXTSTATUS;
		end if;
	end process;
	
	process(clk, STATUS, imageWidth, imageHeight, x, y)
		-- Calculate square root
		function fixedSqrt(radicand : sfixed)
			return sfixed is
		begin
			return to_sfixed(sqrt( to_real(radicand) ), radicand);
		end fixedSqrt;
		
		-- Calculate arc tan
		function fixedAtan2(first : sfixed; second : sfixed)
			return sfixed is
		begin
			return to_sfixed(arctan(to_real(first), to_real(second)), first);
		end fixedAtan2;
		
		-- Calculate cosine
		function fixedCos(number : sfixed)
			return sfixed is
		begin
			return to_sfixed(cos(to_real(number)), number);
		end fixedCos;
		
		-- Calculate sine
		function fixedSin(number : sfixed)
			return sfixed is
		begin
			return to_sfixed(sin(to_real(number)), number);
		end fixedSin;
	
		-- Variables used
		variable fixedOne				: sfixed(7 downto -8);
		variable fixedX				: sfixed(7 downto -8);	
		variable fixedY				: sfixed(7 downto -8);
		variable fixedImageWidth	: sfixed(7 downto -8);	
		variable fixedImageHeight	: sfixed(7 downto -8);	
		
		variable yFactor				: sfixed(7 downto -8);
		variable yOffset				: sfixed(7 downto -8);
		variable normalizedY			: sfixed(7 downto -8);
		variable normalizedY2		: sfixed(7 downto -8);
		
		variable xFactor				: sfixed(7 downto -8);
		variable xOffset				: sfixed(7 downto -8);
		variable normalizedX			: sfixed(7 downto -8);
		variable normalizedX2		: sfixed(7 downto -8);
		
		variable distanceRadicand		: sfixed(7 downto -8);
		variable distanceToCenter		: sfixed(7 downto -8);
		variable newDistanceToCenter	: sfixed(7 downto -8);
		
		variable angleTheta			: sfixed(7 downto -8);
		variable newNormalizedX		: sfixed(7 downto -8);
		variable newNormalizedY		: sfixed(7 downto -8);
		
		variable x2						: integer := 0;
		variable y2						: integer := 0;
	begin
		if (rising_edge(clk)) then
			IF STATUS = "00000000" THEN
				
				fixedOne 			:= to_sfixed(1, fixedOne);
				
				fixedX 				:= to_sfixed(signed(x), fixedX);
				fixedY 				:= to_sfixed(signed(y), fixedY);
				fixedImageWidth 	:= to_sfixed(signed(imageWidth), fixedImageWidth);
				fixedImageHeight 	:= to_sfixed(signed(imageHeight), fixedImageWidth);
				
				yFactor 				:= to_sfixed(2.0, yFactor);
				yOffset 				:= to_sfixed(1.0, yOffset);
				normalizedY 		:= resize(yFactor * fixedY / fixedImageHeight - yOffset, normalizedY);
				normalizedY2 		:= resize(normalizedY * normalizedY, normalizedY2);
							
				xFactor 				:= to_sfixed(2.5, xFactor);
				xOffset 				:= to_sfixed(1.0, xOffset);
				normalizedX 		:= resize(xFactor * fixedX / fixedImageWidth - xOffset, normalizedX);
				normalizedX2 		:= resize(normalizedX * normalizedX, normalizedX2);
				
				distanceRadicand 	:= resize(normalizedX2 + normalizedY2, distanceToCenter);

				--distanceToCenter 	:= fixedSqrt(distanceRadicand);
				distanceToCenter 	:= to_sfixed(0.5, fixedOne);
				if (distanceToCenter > 0 and distanceToCenter <= fixedOne)
				then
					newDistanceToCenter := resize(
						(distanceToCenter + (fixedOne - fixedSqrt(fixedOne - distanceToCenter * distanceToCenter))) / 2
					, fixedOne);
					
					if (newDistanceToCenter <= fixedOne)
					then
						angleTheta := fixedAtan2(normalizedY, normalizedX);
						
						newNormalizedX := resize(newDistanceToCenter * fixedCos(angleTheta), newNormalizedX);
						newNormalizedY := resize(newDistanceToCenter * fixedSin(angleTheta), newNormalizedX);
						--((newNormalizedX + 1.0) * (width / 2.0))
						x2 := (to_integer(
							resize((newNormalizedX + fixedOne) * (fixedImageWidth / to_sfixed(2.0, fixedOne)), fixedOne)
						));
						
						y2 := (to_integer(
							resize((newNormalizedY + fixedOne) * (fixedImageHeight / to_sfixed(2.0, fixedOne)), fixedOne)
						));						
						newPosition <= std_logic_vector(to_signed(y2 * to_integer(signed(imageWidth)) + x2, newPosition'length)); 
					end if;
				else
					newPosition <= "000000000";
				end if;
				
				-- Convert to REAL, calculate atan2 and convert back
				--angleTheta 		:= to_sfixed(arctan(real(5.5), real(6.1)), output);
				--newNormalizedX := resize()
				
				NXTSTATUS <= "00000001";
			END IF;
		end if;
	end process;

end architecture HSB_PERIP; -- of example_peripheral