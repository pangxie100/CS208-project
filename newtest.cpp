namespace boost {
	namespace serialization {
 
		template<class Archive>
		void serialize(Archive & ar, cv::KeyPoint & k, const unsigned int version)
		{
			ar & k.angle;
			ar & k.class_id;
            ar & k.octave;
            ar & k.pt;
            ar & k.response;
            ar & k.size;
		}
 
	} // namespace serialization
} // namespace boost