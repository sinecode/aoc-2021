def main
  if ARGV.length == 0
    STDERR.puts('Please provide the input file as argument')
    exit
  end
  input_file = ARGV[0]
  file_content = File.read(input_file)

  points = []
  fold = nil

  file_content.strip.each_line do |line|
    line.strip!
    next if line.length == 0

    if line.include?('fold along x=')
      fold = XFold.new(line.gsub('fold along x=', '').to_i)
      break
    elsif line.include?('fold along y=')
      fold = YFold.new(line.gsub('fold along y=', '').to_i)
      break
    else
      points.append(line.split(',').map(&:to_i))
    end
  end

  up_fold, bottom_fold = fold.split(points)
  puts fold.fold(up_fold, bottom_fold).length
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
        up_fold.append(point)
      elsif split_coord(point) > @coord
        bottom_fold.append(mirror_point(point))
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

  def mirror_point(point)
    [@coord - (point[0] - @coord), point[1]]
  end
end

class YFold < Fold
  def split_coord(point)
    point[1]
  end

  def mirror_point(point)
    [point[0], @coord - (point[1] - @coord)]
  end
end


main if $0 == __FILE__
