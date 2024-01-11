using Plots

timestamps = []
states = [] 
lates = []


###read
open("/proc/motor_ctrl_log") do io
    while !eof(io)
        line = readline(io)
        timestamp, state, late = split(line, " ")
        append!(timestamps, parse(Int, timestamp) / 2000000)
        append!(states, parse(Int, state))
        append!(lates, parse(Int, late))
    end
end

print("\noutput = ",numbers)

close(file)


plot(timestamps, states, label="PWM Signal", linewidth=2)
