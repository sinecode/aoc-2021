def main
  input_file = File.join(File.expand_path('..', Dir.pwd), 'in')
  content = File.read(input_file)
  commands = content.split("\n")
  horizontal_pos = 0
  depth = 0
  aim = 0
  commands.each do |command|
    direction, steps = command.split
    if direction == 'forward'
      horizontal_pos += steps.to_i
      depth += aim * steps.to_i
    elsif direction == 'down'
      aim += steps.to_i
    elsif direction == 'up'
      aim -= steps.to_i
    else
      raise "Unknown direction: #{direction}"
    end
  end
  puts horizontal_pos * depth
end

main if __FILE__ == $0
