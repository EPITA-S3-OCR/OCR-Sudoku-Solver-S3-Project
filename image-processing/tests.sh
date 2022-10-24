for file in image-processing/images/*.jpg
do 
echo "Test $file"
timeout 4 image-processing/main $file
if [ $? -eq 1 ] 
then
  echo "fail"
else
  echo "succes"
fi
done

