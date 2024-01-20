using Plots

jitters = []
states = []
channels = []

###read
open("/proc/motor_ctrl_log") do io
    while !eof(io)
        line = readline(io)
        jitter, state, channel = split(line, " ")
        append!(jitters, parse(Int, jitter))
        append!(states, parse(Int, state))
        append!(channels, parse(Int, channel))
    end
end

print("\noutput = ", jitters)

jitters = jitters
plotlyjs()
plot(jitters, label="Jitter", linewidth=2, show=true)
