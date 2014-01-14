LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all; 


-- The entity of your testbench. No ports declaration in this case. 
ENTITY fisheyeSim IS
END ENTITY;


ARCHITECTURE testbench OF fisheyeSim IS
  
-- The component declaration should match your entity.
-- It is very important that the name of the component and the ports
-- match your entity!
COMPONENT HSB IS
PORT (
   clk 			: in std_logic;
	imageWidth	: in std_logic_vector(8 downto 0);
	imageHeight	: in std_logic_vector(8 downto 0);
	x				: in std_logic_vector(8 downto 0);
	y				: in std_logic_vector(8 downto 0)
);
END COMPONENT;

-- Signal declaration. These signals are used to drive your inputs and
-- store results (if required).
SIGNAL clk_tb  		: std_logic;
signal clockBool 		: boolean 	:= false;
signal imageWidth_tb : std_logic_vector(8 downto 0)	:= "000001000";
signal imageHeight_tb: std_logic_vector(8 downto 0)	:= "000000010";
signal x_tb 			: std_logic_vector(8 downto 0)	:= "000000010";
signal y_tb 			: std_logic_vector(8 downto 0)	:= "000000100";

BEGIN
  
-- A port map is in this case nothing more than a construction to 
-- connect your entity ports with your signals.
peripheral: HSB PORT MAP (clk => clk_tb, imageWidth => imageWidth_tb, imageHeight => imageHeight_tb,
	x => x_tb, y => y_tb);  
  
	clockTicker: process
	begin    
		FOR I IN 0 TO 10000 LOOP
			wait for 1 ns;
			clockBool <= not clockBool;
		end loop;
	end process;
  
	process (clockBool)
	begin
		if (clockBool) then
			clk_tb <= '1';
		else
			clk_tb <= '0';
		end if;
	end process;

END ARCHITECTURE; 