using Distributions
using StatsPlots

jitters = []
states = []
channels = []

while true
    jitter_new = []
    state_new = []
    channel_new = []

    open("/proc/motor_ctrl_log") do io
        while !eof(io)
            line = readline(io)
            jitter, state, channel = split(line, " ")
            append!(jitter_new, parse(Float64, jitter) / 1000)
            append!(state_new, parse(Float64, state))
            append!(channel_new, parse(Float64, channel))
        end
    end

    if isempty(jitters) || (last(jitter_new) != last(jitters))
        append!(jitters, jitter_new)
        append!(states, state_new)
        append!(channels, channel_new)
    end

    p1 = plot(jitters, label="Jitter", formatter=:plain, xlabel="Count", ylabel="Microseconds", title="Jitters")

    global jitters = convert.(Float64, jitters)

    fit_distribution = fit(Normal{Float64}, jitters)
    p2 = histogram(jitters, label="Jitters", alpha=0.5, normed=true, formatter=:plain)
    plot!(p2, fit_distribution, label="Fitted Normal Distribution", xlabel="Microseconds", ylabel="Frequency", title="Normal Distribution")

    combined_plot = plot(p1, p2, layout=(2, 1))
    display(combined_plot)
end
