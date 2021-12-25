def main
  if ARGV.length == 0
    STDERR.puts('Please provide the input file as argument')
  end
  input_file = ARGV[0]
  file_content = File.read(input_file)
  initial_positions = file_content.split(',').map(&:to_i)
  min_pos = initial_positions.min
  max_pos = initial_positions.max
  min_total_fuel = Float::INFINITY
  min_pos.upto(max_pos).each do |pos|
    total_fuel = initial_positions.map { |initial_pos| (initial_pos - pos).abs }.sum
    if total_fuel < min_total_fuel
      min_total_fuel = total_fuel
    end
  end
  puts min_total_fuel
end

main if __FILE__ == $0

