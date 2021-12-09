class Position
  attr_accessor :horizontal_pos, :depth

  def initialize(horizontal_pos:, depth:)
    @horizontal_pos = horizontal_pos.to_i
    @depth = depth.to_i
  end
end

class Move
  def initialize(steps)
    @steps = steps.to_i
  end
end

class Forward < Move
  def move(pos)
    pos.horizontal_pos += @steps
  end
end

class Down < Move
  def move(pos)
    pos.depth += @steps
  end
end

class Up < Move
  def move(pos)
    pos.depth -= @steps
  end
end

def move_from_command(command)
  direction, steps = command.split
  case direction
  when 'forward'
    Forward.new(steps)
  when 'down'
    Down.new(steps)
  when 'up'
    Up.new(steps)
  else
    raise StandardError.new("Unknown direction: #{direction}")
  end
end

def main
  input_file = File.join(File.expand_path('..', Dir.pwd), 'in')
  content = File.read(input_file)
  commands = content.split("\n")
  pos = Position.new(horizontal_pos: 0, depth: 0)
  commands.each do |command|
    move = move_from_command(command)
    move.move(pos)
  end
  puts pos.horizontal_pos * pos.depth
end

main if __FILE__ == $0
