def gener(n)
	arr = Array.new()
	rnd = Random.new()
	f = File.new("/home/comp/test.txt", "w")
	0.upto(n) do |i|
		f.write(rnd.rand(100).to_s + ' ')
		# arr.push(rnd.rand(1000000000))
	end
end

def check()
	data = Array.new()
	File.open("/home/comp/test.txt", "r").each_line do |line|
  		# name: "Angela"    job: "Writer"    ...
  		data = line.split(' ').map { |s| s.to_i }
	end
	p data
	data.sort!()
	p data

	f = File.new("/home/comp/ruby.txt", "w")
	0.upto(data.size()) do |i|
		f.write(data[i].to_s + ' ')
		# arr.push(rnd.rand(1000000000))
	end
end

check()