library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity PausablePWM is
    Port ( 
        clk     : in  STD_LOGIC;
        btn     : in  STD_LOGIC;                     -- The "Pause" Button
        en      : in  STD_LOGIC_VECTOR(2 downto 0);  -- 3-bit enable vector
        pwm_out : out STD_LOGIC_VECTOR(2 downto 0)
    );
end PausablePWM;

architecture Behavioral of PausablePWM is
    signal counter   : unsigned(7 downto 0) := (others => '0');
    signal count_up  : boolean := true;
    signal pwm_raw   : std_logic;

begin

    -----------------------------------------------------------
    -- Process 1: Counter with Pause Logic
    -----------------------------------------------------------
    process(clk)
    begin
        if rising_edge(clk) then
            -- ONLY update the counter if the button is NOT pressed.
            -- If btn = '1', we skip this block, "freezing" the counter value.
            if btn = '0' then 
                if count_up then
                    counter <= counter + 1;
                    if counter = 255 then
                        count_up <= false;
                    end if;
                else
                    counter <= counter - 1;
                    if counter = 0 then
                        count_up <= true;
                    end if;
                end if;
            end if; -- End of Pause check
        end if;
    end process;

    -----------------------------------------------------------
    -- Process 2: Output Logic
    -----------------------------------------------------------
    -- 1. Determine the state based on the frozen or moving counter
    pwm_raw <= '1' when (counter < 128) else '0';

    -- 2. Output Gating
    --    We removed 'btn' from here. Now, if the counter is frozen 
    --    at a High state, the output STAYS High.
    pwm_out(0) <= pwm_raw when en(0) = '1' else '0';
    pwm_out(1) <= pwm_raw when en(1) = '1' else '0';
    pwm_out(2) <= pwm_raw when en(2) = '1' else '0';

end Behavioral;