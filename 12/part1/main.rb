class Cave
  attr_reader :label

  def initialize(label)
    @label = label.strip
    @visit_count = 0
  end

  def hash
    @label.hash
  end

  def eql?(other)
    hash == other.hash
  end

  def start?
    @label == 'start'
  end

  def end?
    @label == 'end'
  end

  def visit
    raise "Can't visit again this cave" unless can_visit?

    @visit_count += 1
  end

  def can_visit?
    @visit_count < max_visits
  end
end

class SmallCave < Cave
  def max_visits
    1
  end
end

class BigCave < Cave
  def max_visits
    FLOAT::Infinity
  end
end

class CaveSystem
  def initialize
    @system = {}
  end

  def self.parse(data)
    cave_system = CaveSystem.new
    data.strip.each_line do |line|
      first_label, second_label = line.split('-')
      first_cave, second_cave = cave_from_label(first_label), cave_from_label(second_label)
      cave_system.add(first_cave)
      cave_system.add(second_cave)
      cave_system.add_neighbor(first_cave, second_cave)
      cave_system.add_neighbor(second_cave, first_cave)
    end
    return cave_system
  end

  def add(cave)
    @system[cave] = [] unless @system.has_key?(cave)
  end

  def add_neighbor(cave, neighbor)
    return unless @system.has_key?(cave)

    @system[cave] << neighbor unless @system[cave].include?(neighbor)
  end

  def caves
    @system.keys
  end

  def neighbors(cave)
    @system[cave]
  end

  def display
    @system.each do |cave, neighbors|
      print "#{cave.label} "
      neighbors.each{ |n| print " #{n.label}" }
      print "\n"
    end
    nil
  end

  private

    def self.cave_from_label(label)
      label.upcase == label ? BigCave.new(label) : SmallCave.new(label)
    end
end

def main
  if ARGV.length == 0
    STDERR.puts('Please provide the input file as argument')
    exit
  end
  input_file = ARGV[0]
  file_content = File.read(input_file)
  cave_system = CaveSystem.parse(file_content)
  puts cave_system.display
  paths = []
  stack = []
  start_cave = cave_system.caves.select(&:start?).first
  puts start_cave.label
end

main if $0 == __FILE__
