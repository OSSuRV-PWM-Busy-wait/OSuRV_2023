using Distributions
using StatsPlots

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

fit_distribution = fit(Normal{Int}, convert.(Int, jitters))

println("\nMean: $(mean(fit_distribution)), Standard Deviation: $(std(fit_distribution))")

histogram(jitters, label="Jitters[ns]", alpha=0.5, normed=true)
plot!(fit_distribution, label="Fitted Normal Distribution")
