using Distributions
using StatsPlots

jitters = []
states = []
channels = []

open("/proc/motor_ctrl_log") do io
    while !eof(io)
        line = readline(io)
        jitter, state, channel = split(line, " ")
        append!(jitters, parse(Int, jitter))
        append!(states, parse(Int, state))
        append!(channels, parse(Int, channel))
    end
end

p1 = plot(jitters, label="Jitter", formatter=:plain, linewidth=2)
fit_distribution = fit(Normal{Int}, convert.(Int, jitters))
p2 = histogram(jitters, label="Jitters[ns]", alpha=0.5, normed=true)
plot!(p2, fit_distribution, formatter=:plain, label="Fitted Normal Distribution")

combined_plot = plot(p1, p2, layout=(2, 1))
display(combined_plot)

