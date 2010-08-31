# Set Message Box as output system
Output.msgbox

#Assigning a background
Graphics.backcolor = Color.new(0, 0, 0)

Graphics.frame_rate = 60

#Testing
sprite = Sprite.new
sprite.bitmap = Bitmap.new("Graphics/test")
sprite.x = (Graphics.width - sprite.width) / 2
sprite.y = (Graphics.height - sprite.height) / 2

Graphics.brightness = 0
Graphics.backcolor = Color.new(255, 255, 255)
Graphics.update
Graphics.fadein(255)

loop do
	Graphics.update
	Input.update
	sprite.update

	sprite.bitmap.blur

	if Input.press?(Keys::RETURN)
		sprite.bitmap = Bitmap.new("Graphics/test")
		Graphics.brightness = 0
		Graphics.update
		Graphics.fadein(255)
	end
end
