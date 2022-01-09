def main
  if ARGV.length == 0
    STDERR.puts('Please provide the input file as argument')
    exit
  end
  input_file = ARGV[0]
  file_content = File.read(input_file)

  points = []
  folds = []

  file_content.strip.each_line do |line|
    line.strip!
    next if line.length == 0

    if line.include?('fold along x=')
      folds.append(XFold.new(line.gsub('fold along x=', '').to_i))
    elsif line.include?('fold along y=')
      folds.append(YFold.new(line.gsub('fold along y=', '').to_i))
    else
      points.append(line.split(',').map(&:to_i))
    end
  end

  folds.each do |fold|
    up_fold, bottom_fold = fold.split(points)
    points = fold.fold(up_fold, bottom_fold)
  end
  #points.each { |p| puts "(#{p[0]},#{p[1]})" }
  draw(points)
  puts points.length
end

class Fold
  def initialize(coord)
    @coord = coord
  end

  def split(points)
    up_fold = []
    bottom_fold = []
    points.each do |point|
      if split_coord(point) < @coord
        up_fold.append(if_less(point))
      elsif split_coord(point) > @coord
        bottom_fold.append(if_greater(point))
      # discard the points on y_fold
      end
    end
    [up_fold, bottom_fold]
  end

  def fold(a, b)
    (a + b).uniq
  end
end

class XFold < Fold
  def split_coord(point)
    point[0]
  end

  def if_less(point)
    point
  end

  def if_greater(point)
    [@coord - (point[0] - @coord), point[1]]
  end
end

class YFold < Fold
  def split_coord(point)
    point[1]
  end

  def if_less(point)
    point
  end

  def if_greater(point)
    [point[0], @coord - (point[1] - @coord)]
  end
end

def draw(points)
  max_x = points.map { |p| p[0] }.max
  max_y = points.map { |p| p[1] }.max
  (0..max_y).each do |y|
    (0..max_x).each do |x|
      if points.include?([x, y])
        print("#")
      else
        print(".")
      end
    end
    print("\n")
  end
end


main if $0 == __FILE__
