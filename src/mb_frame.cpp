#include "mb_frame.h"
#include "mb_piece.h"

MBFrame::MBFrame(){

}

MBFrame::MBFrame(std::string, std::string frame_type, double px, double py, int id_piece, int frame_id, int mask){
    if(FRAME) printf("Construindo frame\n");
	m_correct_piece = id_piece;
    m_sprite_speed = 0;
    m_sprite_counter = 0;
    m_start = -1;
	m_is_right = false;
	m_type = frame_type;
    m_id = frame_id;
	m_filled = false;

    set_priority(2);

    string mask_string = "";

    if(m_type == "activity"){
        m_height = 144;
        m_width = m_draw_width = 253;
        m_draw_height = 163;
        m_minimum_area = 16000;
        mask_string = "_" + to_string(mask);
    }
	
    else if(m_type == "in"){
		m_height = 72;
        m_width = m_draw_width = 66;
        m_draw_height = 91;
        m_minimum_area = 2500;
	}

    else if(m_type == "intermediary"){
        m_height = m_draw_height = 80;
        m_width = m_draw_width = 80;
        m_minimum_area = 2500;
        set_priority(4);
    }

    else if(m_type == "out1"){
		m_height = m_draw_height = 65;
        m_width = m_draw_width = 72;
        m_minimum_area = 2500;
	}

    else if(m_type == "out2"){
        m_height = m_draw_height = 65;
        m_width = m_draw_width = 73;
        m_minimum_area = 2500;
    }

    else if(m_type == "treatment"){
		m_height = m_draw_height = 67;
        m_width = m_draw_width = 143;
        m_minimum_area = 500;
	}

    else{
        printf("Invalid type of piece\n");
        exit(0);
    }

    m_y = py;
    m_x = px;

	m_texture = resources::get_texture("frames/frame_" + m_type + mask_string + ".png");
		
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    m_active = true;
    m_piece = nullptr;

    physics::register_object(this);
    event::register_listener(this);
    if(FRAME) printf("Construiu frame\n");
}

MBFrame::~MBFrame(){
    event::unregister_listener(this);
    physics::unregister_object(this);
}

double MBFrame::x() const{ return m_x; }
double MBFrame::y() const{ return m_y; }
int MBFrame::id() const{ return m_id; }
string MBFrame::type() const{ return m_type; }
const MBPiece * MBFrame::piece() const{ return m_piece; }
double MBFrame::height(){ return m_height; }
double MBFrame::width(){ return m_width; }
bool MBFrame::is_right() { return m_is_right; }
shared_ptr<Texture> MBFrame::texture(){ return m_texture; }
double MBFrame::minimum_area() const{ return m_minimum_area; }
bool MBFrame::filled() { return m_filled; }

void MBFrame::set_x(double cx) { m_x = cx; }
void MBFrame::set_y(double cy) { m_y = cy; }
void MBFrame::set_height(double ch) { m_height = ch; }    
void MBFrame::set_right(bool is_r){ m_is_right = is_r; }
void MBFrame::set_filled(bool is_filled){ m_filled = is_filled; }

void MBFrame::register_self(int current_x){
    m_x = current_x;
    m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
    m_active = true;
    physics::register_object(this);
}

bool MBFrame::on_event(const GameEvent&){
    return false;
}

bool MBFrame::active() const{
    return m_active;
}

pair<double, double> MBFrame::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& MBFrame::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& MBFrame::hit_boxes() const{
    return l;
}

void MBFrame::on_collision(const Collidable * c_piece, const Rectangle& rectangle, const unsigned now, const unsigned){
	if(auto p = dynamic_cast<const MBPiece *>(c_piece)){
        int id_piece = p->id();

        if(rectangle.area() >= m_minimum_area && not p->following() && (now - p->last_following()) < 20 && m_piece == nullptr && p->type() == m_type){
			if(id_piece == m_correct_piece){
				m_is_right = true;
			}else{
				m_is_right = false;
			}

            m_piece = p;
			set_filled(true);
        }
    }
}

void MBFrame::update_self(unsigned now, unsigned) {
    if(FRAME) printf("Entrou update frame\n");
    if(m_start == -1){
        m_start = now;
    }

    m_bounding_box = Rectangle(m_x + m_width/2, m_y + m_height/2, m_width, m_height);
    l.clear();

    if(m_type != "treatment"){
        l.insert(l.begin(), m_bounding_box);
    }else{
        l.insert(l.begin(), Rectangle(m_x + 35.0/2, m_y + 27.0/2, 35, 27));
        l.insert(l.begin(), Rectangle(m_x + 143.0/2, m_y + 40 + 40.0/2, 143, 40));
        l.insert(l.begin(), Rectangle(m_x + 103 + 40.0/2, m_y + 27.0/2, 40, 27));
    }

    if(m_piece != nullptr){
        if(m_piece->frame_id() != m_id){
            m_piece = nullptr;
			set_filled(false);
			set_right(false);
        }
    }

    m_start = now;
    if(FRAME) printf("Saiu update frame\n");
}

void MBFrame::draw_self(Canvas* canvas, unsigned, unsigned) {
    if(FRAME) printf("Entrando draw_self frame\n");
    if(m_active) canvas->draw(m_texture.get(), Rectangle(m_width * ((int) m_sprite_counter), 0, m_width, m_height), m_x, m_y);
    if(FRAME) printf("Saindo draw_self frame\n");
}
