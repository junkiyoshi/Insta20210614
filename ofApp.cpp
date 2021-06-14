#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofColor color, tmp_color;
	for (int k = 0; k < 23; k++) {

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
		
		color.setHsb((int)ofRandom(200, 280) % 255, 255,255);

		ofMesh mesh, line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		vector<glm::vec3> right, left;

		glm::vec3 last_location;
		float last_theta;
		int len = 30;
		int head_size = 15;

		for (int i = 0; i < len; i++) {

			auto location = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.005), 0, 1, -325, 325), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350), 0);
			auto next = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -325, 325), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -350, 350), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);

			line.addVertex(left[i]);
			line.addVertex(right[i]);

			if (i > right.size() * 0.5) {
				
				tmp_color.setHsb(color.getHue(), color.getSaturation(), ofMap(i, right.size(), right.size() * 0.5, color.getBrightness(), 16));
			}
			else {

				tmp_color.setHsb(color.getHue(), color.getSaturation(), 32);
			}

			mesh.addColor(tmp_color);
			mesh.addColor(tmp_color);

			line.addColor(color);
			line.addColor(color);
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);

			line.addIndex(i + 0); line.addIndex(i + 2);
			line.addIndex(i + 1); line.addIndex(i + 3);
		}

		auto tmp_head_size = ofMap(len - 2, 0, len, 0, head_size);
		tmp_color.setHsb(color.getHue(), color.getSaturation(), ofMap(len - 2, right.size(), right.size() * 0.5, color.getBrightness(), 16));

		mesh.addVertex(last_location);
		mesh.addColor(tmp_color);
		line.addVertex(last_location);
		line.addColor(color);

		int index = mesh.getNumVertices();
		tmp_color.setHsb(color.getHue(), color.getSaturation(), ofMap(len, right.size(), right.size() * 0.5, color.getBrightness(), 16));
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
			line.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
			mesh.addColor(tmp_color);
			line.addColor(color);
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
			line.addIndex(i + 0); line.addIndex(i + 1);
		}

		mesh.draw();
		line.drawWireframe();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}