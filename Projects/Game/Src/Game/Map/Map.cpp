#include "Map.h"
#include "Utils/FileUtils.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Gamepad/Gamepad.h"
#include "Utils/SoLib/SoLib.h"

void Map::Initialize(const std::string& fileName, bool isHint) {
	mapDatas_ = Lamb::LoadCsv(fileName);
	startMapDatas_ = mapDatas_;
	float x = 0.0f, y = 0.0f;
	y = static_cast<float>(mapDatas_.size());

	Vector2 positioning = {
		static_cast<float>(mapDatas_.front().size() - 1) * kMapSize * 0.5f,
		static_cast<float>(mapDatas_.size() + 2) * kMapSize * 0.5f
	};

	float mapNumX = 0.0f, mapNumY = 0.0f;

	static const std::string wallFileNmae = "Resources/wall.png";
	static const std::string holeFileNmae = "Resources/hole.png";
	static const std::string playerFileNmae = "Resources/player.png";
	static const std::string nodeFileNmae = "Resources/loveNodet.png";
	static const std::string portFileNmae = "Resources/port.png";
	static const std::string heartFileNmae = "Resources/whiteHeart.png";
	static const std::string goalFileNmae = "Resources/fullGoal.png";
	static const std::string golaAnimationFileName = "Resources/openGoal.png";

	std::deque<Vector2> hintsPos;

	for (auto& line : mapDatas_) {
		for (auto& num : line) {
			switch (static_cast<Map::Type>(num)) {
				// 動かない穴
			case Type::kHole:
				holes_.push_back(MapNode());
				holes_.back().Initialize(
					Vector2(mapNumX, mapNumY),
					0x0,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					holeFileNmae,
					holeFileNmae,
					holeFileNmae,
					kMapSize,
					MapNode::Type::kHole
				);
				break;

				// 動かせない壁
			case Type::kWall:
				walls_.push_back(MapNode());
				walls_.back().Initialize(
					Vector2(mapNumX, mapNumY),
					0x0,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					wallFileNmae,
					"Resources/EngineResources/white2x2.png",
					"Resources/EngineResources/white2x2.png",
					kMapSize,
					MapNode::Type::kWall
				);
				break;

			case Type::kPlayer:
				player_.reset(new Player);

				player_->Initialize(
					{
					((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					((kMapSize * 0.5f) + y * kMapSize) - positioning.y
					},
					playerFileNmae, kMapSize,
					Vector2(mapNumX, mapNumY));

				break;

			case Type::kGoalUp:
				goal_.reset(new MapNode);

				goal_->Initialize(
					Vector2(mapNumX, mapNumY),
					0x1,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					goalFileNmae,
					portFileNmae,
					golaAnimationFileName,
					kMapSize,
					MapNode::Type::kGoal
				);
				break;
			case Type::kGoalRight:
				goal_.reset(new MapNode);

				goal_->Initialize(
					Vector2(mapNumX, mapNumY),
					0x2,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					goalFileNmae,
					portFileNmae,
					golaAnimationFileName,
					kMapSize,
					MapNode::Type::kGoal
				);
				break;
			case Type::kGoalDown:
				goal_.reset(new MapNode);

				goal_->Initialize(
					Vector2(mapNumX, mapNumY),
					0x4,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					goalFileNmae,
					portFileNmae,
					golaAnimationFileName,
					kMapSize,
					MapNode::Type::kGoal
				);
				break;
			case Type::kGoalLeft:
				goal_.reset(new MapNode);

				goal_->Initialize(
					Vector2(mapNumX, mapNumY),
					0x8,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					goalFileNmae,
					portFileNmae,
					golaAnimationFileName,
					kMapSize,
					MapNode::Type::kGoal
				);
				break;

			case Type::kNone:
				// 何もしない
				break;

			case Type::kHint:
				hintsPos.push_back(
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y }
				);
				break;

				// Node
			default:
				nodes_.push_back(MapNode());
				nodes_.back().Initialize(
					Vector2(mapNumX, mapNumY),
					num,
					{ ((kMapSize * 0.5f) + x * kMapSize) - positioning.x,
					  ((kMapSize * 0.5f) + y * kMapSize) - positioning.y },
					nodeFileNmae,
					portFileNmae,
					heartFileNmae,
					kMapSize,
					MapNode::Type::kMove
				);
				break;
			}

			x++;
			mapNumX++;
		}
		x = 0.0f;
		y--;
		mapNumX = 0.0f;
		mapNumY++;
	}

	allPortCount_ = 1;

	for (const auto& i : nodes_) {
		for (size_t j = 0; j < i.GetIsPort().size(); j++) {
			if (i.GetIsPort()[j]) {
				allPortCount_++;
			}
		}
	}

	lines_.resize(allPortCount_ / 2);

	for (auto& i : lines_) {
		i.Init();
	}

	portCount_ = 0;

	isCanGoal_ = false;

	isClear_ = false;

	//isGoalAnimationFinish_ = false;

	Lines();

	isLineConect_ = false;
	isLineDisconect_ = false;

	AudioManager::GetInstance()->Load("./Resources/Sound/SE/line_connect.wav");
	AudioManager::GetInstance()->Load("./Resources/Sound/SE/line_cancel.wav");
	lineConectSe_ = AudioManager::GetInstance()->Get("./Resources/Sound/SE/line_connect.wav");
	lineDisconectSe_ = AudioManager::GetInstance()->Get("./Resources/Sound/SE/line_cancel.wav");
	isDie_ = false;

	isPreDie_ = false;

	mapStart_ = std::chrono::steady_clock::now();
	canMoveTimeDuration_ = std::chrono::milliseconds(300);

	hints_.resize(hintsPos.size());
	auto hitPosItr = hintsPos.begin();
	for (auto& i : hints_) {
		i.Load("Resources/EngineResources/white2x2.png");
		i.scale *= kMapSize;
		i.pos = *hitPosItr;
	}

	isHints_ = isHint;
	hintMaxColor_ = 0xfefecbff;
	hintMinColor_ = 0xfefecb00;
	hintEaseT_ = 0.0f;
	hintEaseSpd_ = 1.0f;
}

void Map::Update() {
	if (player_) {
		player_->SetMove(Vector2::kZero);
	}
	for (auto& i : nodes_) {
		i.moveVec_ = Vector2::kZero;
	}

	bool isPlayerMove = false;
	for (auto &y : mapDatas_) {
		for (auto &x : y) {
			if (0 < x && x < 16 || x == int(Type::kPlayer)) {
				x = 0;
			}
		}
	}

	isPreDie_ = isDie_;

	auto nowTime = std::chrono::steady_clock::now();

	if (player_) {
		if (!isClear_ && !isDie_ && 
			std::chrono::duration_cast<std::chrono::milliseconds>(nowTime-mapStart_) >= canMoveTimeDuration_) {
			isPlayerMove = player_->Move();
		}
		else {
			player_->SetMove(Vector2::kZero);
		}

		if (isDie_) {
			player_->isMove_ = false;
		}
		Collision();
		Undo(isPlayerMove && player_->GetMoveVec() != Vector2::kZero);

		player_->SetMove(player_->GetMoveVec() * kMapSize);

		player_->Update();

		mapDatas_[int(player_->mapPos_.y)][int(player_->mapPos_.x)] = int(Type::kPlayer);
	}

	for (auto &i : nodes_) {
		if (player_ && isPlayerMove && player_->GetMoveVec() != Vector2::kZero) {
			i.SaveThisFramePos();
		}
		i.Update();
		mapDatas_[int(i.GetMapNum().y)][int(i.GetMapNum().x)] = int(i.GetIsPort().to_ulong());
	}

	for (auto &i : holes_) {
		i.Update();
	}

	for (auto &i : walls_) {
		i.Update();
	}

	for (auto& i : lines_) {
		i.Update();
	}

	if (goal_) {
		goal_->Update();
	}

	if (isPlayerMove) {
		Lines();
	}

	if (player_ && goal_ && goal_->Collision(player_->GetMoveToPos())) {
		isClear_ = true;
		goal_->AnimaionStart(2.0f);
	}

	if (player_) {
		player_->Animation(/*isPlayerMove, false*/);
	}

	if (isHints_) {
		for (auto& i : hints_) {
			i.color = SoLib::ColorLerp(hintMinColor_, hintMaxColor_, hintEaseT_);
		}
		hintEaseT_ += hintEaseSpd_ * ImGui::GetIO().DeltaTime;
		if (hintEaseT_ >= 1.0f) {
			hintEaseT_ = 1.0f;
			hintEaseSpd_ *= -1.0f;
		}
		else if(hintEaseT_ <= 0.0f) {
			hintEaseT_ = 0.0f;
			hintEaseSpd_ *= -1.0f;
		}
	}
}

void Map::Draw(const Mat4x4 &cameraMat) {
	for (auto &i : holes_) {
		i.Draw(cameraMat);
	}

	for (auto &i : walls_) {
		i.Draw(cameraMat);
	}

	if (isHints_) {
		for (auto& i : hints_) {
			i.Draw(cameraMat);
		}
	}

	if (player_ && !isClear_ && !isDie_) {
		player_->Draw(cameraMat);
	}
	if (isCanGoal_) {
		if (goal_) {
			goal_->Draw(cameraMat);
		}

		for (auto& i : lines_) {
			i.Draw(cameraMat);
		}
	}
	else {
		for (auto& i : lines_) {
			i.Draw(cameraMat);
		}
		if (goal_) {
			goal_->Draw(cameraMat);
		}
	}



	for (auto& i : nodes_) {
		i.Draw(cameraMat);
	}


	if (isLineConect_ && lineConectSe_) {
		lineConectSe_->Start(1.0f, false);
		isLineConect_ = false;
	}
	else if (isLineDisconect_ && lineDisconectSe_) {
		lineDisconectSe_->Start(1.0f, false);
		isLineDisconect_ = false;
	}
}

void Map::Collision() {
	assert(player_);
	auto &&playerMoveVec = player_->GetMoveVec() * kMapSize;
	auto &&playerNextPos = player_->pos_ + playerMoveVec;

	// もし移動先が壁だったら移動量をゼロにする
	for (auto &wall : walls_) {
		if (wall.Collision(playerNextPos)) {
			player_->SetMove(Vector2::kZero);
			break;
		}
	}

	// もし移動先が穴だったら移動量をゼロにする
	for (auto &holl : holes_) {
		if (holl.Collision(playerNextPos)) {
			player_->SetMove(Vector2::kZero);
			break;
		}
	}

	// もし移動先が箱だったら箱も移動する
	// 動かす箱の移動先が穴か壁なら移動量をゼロにする
	auto moveNodeItr = nodes_.end();
	for (auto i = nodes_.begin(); i != nodes_.end(); i++) {
		if (i->Collision(playerNextPos)) {
			i->moveVec_ = playerMoveVec;
			moveNodeItr = i;
			break;
		}
	}

	// もしゴールフラグがfalseかつ移動先がゴールなら移動量をゼロにする
	if (!isCanGoal_ && goal_ && goal_->Collision(playerNextPos)) {
		player_->SetMove(Vector2::kZero);
	}

	if (moveNodeItr == nodes_.end()) {
		// もし移動先に赤い糸があったら移動量をゼロにする
		Vector2 max;
		Vector2 min;
		for (auto &i : linePos_) {
			max.x = i.first.x < i.second.x ? i.second.x : i.first.x;
			max.y = i.first.y < i.second.y ? i.second.y : i.first.y;
			min.x = i.first.x < i.second.x ? i.first.x : i.second.x;
			min.y = i.first.y < i.second.y ? i.first.y : i.second.y;

			if ((i.first - i.second) == Vector2::kZero) {
				continue;
			}
			else if (i.first.y == i.second.y && playerNextPos.y == i.first.y) {
				if (min.x < playerNextPos.x && playerNextPos.x < max.x) {
					player_->SetMove(Vector2::kZero);
					break;
				}
			}
			else if (i.first.x == i.second.x && playerNextPos.x == i.first.x) {
				if (min.y < playerNextPos.y && playerNextPos.y < max.y) {
					player_->SetMove(Vector2::kZero);
					break;
				}
			}
			else if (i.first.y == i.second.y && player_->GetMoveToPos().y == i.first.y) {
				if (min.x < player_->GetMoveToPos().x && player_->GetMoveToPos().x < max.x) {
					player_->SetMove(Vector2::kZero);
					isDie_ = true;
					break;
				}
			}
			else if (i.first.x == i.second.x && player_->GetMoveToPos().x == i.first.x) {
				if (min.y < player_->GetMoveToPos().y && player_->GetMoveToPos().y < max.y) {
					player_->SetMove(Vector2::kZero);
					isDie_ = true;
					break;
				}
			}
		}
	}

	// 動かす箱があったら
	else if (moveNodeItr != nodes_.end()) {
		// 動かす箱の移動先
		Vector2 moveNodeNextPos = moveNodeItr->pos_ + moveNodeItr->moveVec_;

		// もし移動先が壁だったら移動量をゼロにする
		for (auto &wall : walls_) {
			if (wall.Collision(moveNodeNextPos)) {
				moveNodeItr->moveVec_ = Vector2::kZero;
				player_->SetMove(Vector2::kZero);
				player_->CannotMove();
				break;
			}
		}

		for (auto i = nodes_.begin(); i != nodes_.end(); i++) {
			if (i == moveNodeItr) {
				continue;
			}
			else if (i->Collision(moveNodeNextPos)) {
				moveNodeItr->moveVec_ = Vector2::kZero;
				player_->SetMove(Vector2::kZero);
				player_->CannotMove();
			}
		}

		// もし移動先が穴だったら移動量をゼロにする
		for (auto &holl : holes_) {
			if (holl.Collision(moveNodeNextPos)) {
				moveNodeItr->moveVec_ = Vector2::kZero;
				player_->SetMove(Vector2::kZero);
				player_->CannotMove();
				break;
			}
		}

		// もし移動先がゴールだったら
		if (goal_) {
			if (goal_->Collision(moveNodeNextPos)) {
				moveNodeItr->moveVec_ = Vector2::kZero;
				player_->SetMove(Vector2::kZero);
				player_->CannotMove();
			}
		}

		Vector2 max;
		Vector2 min;
		// もし移動先が赤い糸だったら
		for (auto &i : linePos_) {
			max.x = i.first.x < i.second.x ? i.second.x : i.first.x;
			max.y = i.first.y < i.second.y ? i.second.y : i.first.y;
			min.x = i.first.x < i.second.x ? i.first.x : i.second.x;
			min.y = i.first.y < i.second.y ? i.first.y : i.second.y;

			if ((i.first - i.second) == Vector2::kZero) {
				continue;
			}
			else if (moveNodeItr->pos_ != Vector2(i.first.x, i.first.y)
				&& moveNodeItr->pos_ != Vector2(i.second.x, i.second.y)) {
				if (i.first.y == i.second.y && moveNodeNextPos.y == i.first.y) {
					if (min.x < moveNodeNextPos.x && moveNodeNextPos.x < max.x) {
						moveNodeItr->moveVec_ = Vector2::kZero;
						player_->SetMove(Vector2::kZero);
						player_->CannotMove();
						break;
					}
				}
				else if (i.first.x == i.second.x && moveNodeNextPos.x == i.first.x) {
					if (min.y < moveNodeNextPos.y && moveNodeNextPos.y < max.y) {
						moveNodeItr->moveVec_ = Vector2::kZero;
						player_->SetMove(Vector2::kZero);
						player_->CannotMove();
						break;
					}
				}
				else if (i.first.y == i.second.y && player_->GetMoveToPos().y == i.first.y) {
					if (min.x < player_->GetMoveToPos().x && player_->GetMoveToPos().x < max.x) {
						moveNodeItr->moveVec_ = Vector2::kZero;
						player_->SetMove(Vector2::kZero);
						isDie_ = true;
						break;
					}
				}
				else if (i.first.x == i.second.x && player_->GetMoveToPos().x == i.first.x) {
					if (min.y < player_->GetMoveToPos().y && player_->GetMoveToPos().y < max.y) {
						moveNodeItr->moveVec_ = Vector2::kZero;
						player_->SetMove(Vector2::kZero);
						isDie_ = true;
						break;
					}
				}
			}
		}
	}

	//if (player_->GetMoveVec() != Vector2::kZero) {
	//	if (moveNodeItr != nodes_.end() && moveNodeItr->moveVec_.x != Vector2::kZero) {
	//		std::swap(
	//			mapDatas_[int(moveNodeItr->GetMapNum().y)][int(moveNodeItr->GetMapNum().x)],
	//			mapDatas_[int(moveNodeItr->GetMapNum().y - moveNodeItr->moveVec_.y)][int(moveNodeItr->GetMapNum().x + moveNodeItr->moveVec_.x)]
	//		);
	//	}

	//	std::swap(
	//		mapDatas_[int(player_->mapPos_.y)][int(player_->mapPos_.x)],
	//		mapDatas_[int(player_->mapPos_.y - player_->GetMoveVec().y)][int(player_->mapPos_.x + player_->GetMoveVec().x)]
	//	);
	//}
}

void Map::Lines() {
	for (auto &i : lines_) {
		i.SetPos(Vector2::kZero, Vector2::kZero, kMapSize);
	}

	Vector2 positioning = {
		static_cast<float>(mapDatas_.front().size() - 1) * kMapSize * 0.5f,
		static_cast<float>(mapDatas_.size() + 2) * kMapSize * 0.5f
	};

	decltype(linePos_) preLinePos = linePos_;

	linePos_.clear();

	for (auto &node : nodes_) {
		const auto &isPorts = node.GetIsPort();

		int32_t x = 0, y = 0;
		Vector2 currentMapNum;

		// 上
		if (isPorts[0]) {
			currentMapNum = node.GetMapNum();
			currentMapNum.y -= 1.0f;
			x = int(currentMapNum.x);
			y = int(currentMapNum.y);

			// マップの端っこぶち当たるまでループ
			while (0 <= y) {
				// もし壁だったら
				if (mapDatas_[y][x] == int(Type::kWall)) {
					break;
				}
				// もしノードだったら
				else if ((1 <= mapDatas_[y][x] && mapDatas_[y][x] <= 15)) {
					// もし向かい合うノードだったら
					if ((mapDatas_[y][x] & 4) == 4) {
						Vector2 startPos = node.GetMoveToPos();
						auto endPosItr = std::find_if(nodes_.begin(), nodes_.end(),
							[&y, &x](const MapNode &num) {
								return num.GetMapNum() == Vector2(float(x), float(y));
							});
						if (endPosItr == nodes_.end()) {
							break;
						}
						Vector2 endPos;
						endPos = endPosItr->GetMoveToPos();
						//もしlinePos_がからかこれから入れるラインのポジションがなかったら
						if (linePos_.empty()
							|| std::find_if(linePos_.begin(), linePos_.end(),
								[&startPos, &endPos](const std::pair<Vector2, Vector2> &num) {
									return
										(startPos == num.first || startPos == num.second)
										&& (endPos == num.first || endPos == num.second);
								}) == linePos_.end()) {
							linePos_.push_back(
								{
									startPos,
									endPos
								}
							);
						}
					}
					break;
				}
				// もしゴールだったら
				else if (goal_ && goal_->GetMapNum() == Vector2(float(x), float(y))) {
					// もしむかい合うノードだったら
					if (goal_->GetIsPort()[2]) {
						Vector2 startPos = node.GetMoveToPos();
						Vector2 endPos = goal_->pos_;
						linePos_.push_back(
							{
								startPos,
								endPos
							}
						);
					}
					break;
				}
				// それ以外
				else {
					y--;
				}
			}
		}
		if (isPorts[1]) {
			currentMapNum = node.GetMapNum();
			currentMapNum.x += 1.0f;
			x = int(currentMapNum.x);
			y = int(currentMapNum.y);

			// マップの端っこぶち当たるまでループ
			while (size_t(x) < mapDatas_.size()) {
				// もし壁だったら
				if (mapDatas_[y][x] == int(Type::kWall)) {
					break;
				}
				// もしノードだったら
				else if ((1 <= mapDatas_[y][x] && mapDatas_[y][x] <= 15)) {
					// もし向かい合うノードだったら
					if ((mapDatas_[y][x] & 8) == 8) {
						Vector2 startPos = node.GetMoveToPos();
						auto endPosItr = std::find_if(nodes_.begin(), nodes_.end(),
							[&y, &x](const MapNode &num) {
								return num.GetMapNum() == Vector2(float(x), float(y));
							});
						if (endPosItr == nodes_.end()) {
							break;
						}
						Vector2 endPos;
						endPos = endPosItr->GetMoveToPos();
						//もしlinePos_がからかこれから入れるラインのポジションがなかったら
						if (linePos_.empty()
							|| std::find_if(linePos_.begin(), linePos_.end(),
								[&startPos, &endPos](const std::pair<Vector2, Vector2> &num) {
									return
										(startPos == num.first || startPos == num.second)
										&& (endPos == num.first || endPos == num.second);
								}) == linePos_.end()) {
							linePos_.push_back(
								{
									startPos,
									endPos
								}
							);
						}
					}
					break;
				}
				// もしゴールだったら
				else if (goal_ && goal_->GetMapNum() == Vector2(float(x), float(y))) {
					// もしむかい合うノードだったら
					if (goal_->GetIsPort()[3]) {
						Vector2 startPos = node.GetMoveToPos();
						Vector2 endPos = goal_->pos_;

						linePos_.push_back(
							{
								startPos,
								endPos
							}
						);
					}
					break;
				}
				// それ以外
				else {
					x++;
				}
			}
		}
		if (isPorts[2]) {
			currentMapNum = node.GetMapNum();
			currentMapNum.y += 1.0f;
			x = int(currentMapNum.x);
			y = int(currentMapNum.y);

			// マップの端っこぶち当たるまでループ
			while (size_t(y) < mapDatas_.size()) {
				// もし壁だったら
				if (mapDatas_[y][x] == int(Type::kWall)) {
					break;
				}
				// もしノードだったら
				else if ((1 <= mapDatas_[y][x] && mapDatas_[y][x] <= 15)) {
					// もし向かい合うノードだったら
					if ((mapDatas_[y][x] & 1) == 1) {
						Vector2 startPos = node.GetMoveToPos();
						auto endPosItr = std::find_if(nodes_.begin(), nodes_.end(),
							[&y, &x](const MapNode &num) {
								return num.GetMapNum() == Vector2(float(x), float(y));
							});
						if (endPosItr == nodes_.end()) {
							break;
						}
						Vector2 endPos;
						endPos = endPosItr->GetMoveToPos();
						//もしlinePos_がからかこれから入れるラインのポジションがなかったら
						if (linePos_.empty()
							|| std::find_if(linePos_.begin(), linePos_.end(),
								[&startPos, &endPos](const std::pair<Vector2, Vector2> &num) {
									return
										(startPos == num.first || startPos == num.second)
										&& (endPos == num.first || endPos == num.second);
								}) == linePos_.end()) {
							linePos_.push_back(
								{
									startPos,
									endPos
								}
							);
						}
					}
					break;
				}
				// もしゴールだったら
				else if (goal_ && goal_->GetMapNum() == Vector2(float(x), float(y))) {
					// もしむかい合うノードだったら
					if (goal_->GetIsPort()[0]) {
						Vector2 startPos = node.GetMoveToPos();
						Vector2 endPos = goal_->pos_;

						linePos_.push_back(
							{
								startPos,
								endPos
							}
						);
					}
					break;
				}
				// それ以外
				else {
					y++;
				}
			}
		}
		if (isPorts[3]) {
			currentMapNum = node.GetMapNum();
			currentMapNum.x -= 1.0f;
			x = int(currentMapNum.x);
			y = int(currentMapNum.y);

			// マップの端っこぶち当たるまでループ
			while (0 <= x) {
				// もし壁だったら
				if (mapDatas_[y][x] == int(Type::kWall)) {
					break;
				}
				// もしノードだったら
				else if ((1 <= mapDatas_[y][x] && mapDatas_[y][x] <= 15)) {
					// もし向かい合うノードだったら
					if ((mapDatas_[y][x] & 2) == 2) {
						Vector2 startPos = node.GetMoveToPos();
						auto endPosItr = std::find_if(nodes_.begin(), nodes_.end(),
							[&y, &x](const MapNode &num) {
								return num.GetMapNum() == Vector2(float(x), float(y));
							});
						if (endPosItr == nodes_.end()) {
							break;
						}
						Vector2 endPos;
						endPos = endPosItr->GetMoveToPos();
						//もしlinePos_がからかこれから入れるラインのポジションがなかったら
						if (linePos_.empty()
							|| std::find_if(linePos_.begin(), linePos_.end(),
								[&startPos, &endPos](const std::pair<Vector2, Vector2> &num) {
									return
										(startPos == num.first || startPos == num.second)
										&& (endPos == num.first || endPos == num.second);
								}) == linePos_.end()) {
							linePos_.push_back(
								{
									startPos,
									endPos
								}
							);
						}
					}
					break;
				}
				// もしゴールだったら
				else if (goal_ && goal_->GetMapNum() == Vector2(float(x), float(y))) {
					// もしむかい合うノードだったら
					if (goal_->GetIsPort()[1]) {
						Vector2 startPos = node.GetMoveToPos();
						Vector2 endPos = goal_->pos_;

						linePos_.push_back(
							{
								startPos,
								endPos
							}
						);
					}
					break;
				}
				// それ以外
				else {
					x--;
				}
			}
		}
	}

	isLineConect_ = false;
	isLineDisconect_ = false;
	if (!linePos_.empty() && portCount_ < linePos_.size() * 2) {
		isLineDisconect_ = true;
	}

	auto lineItr = lines_.begin();
	for (auto &pos : linePos_) {
		if (std::find_if(preLinePos.begin(), preLinePos.end(),
			[&pos](const std::pair<Vector2, Vector2>& num) {
				return (pos.first == num.first
					|| pos.first == num.second)
					&& (pos.second == num.first
						|| pos.second == num.second);
			}
		) == preLinePos.end()) {
			isLineConect_ = true;
		}

		if (lineItr != lines_.end()) {
			if (pos.first.x > pos.second.x || pos.first.y > pos.second.y) {
				std::swap(pos.first, pos.second);
			}
			lineItr->SetPos(pos.first, pos.second, kMapSize);
		}
		lineItr++;
	}

	for (auto& node : nodes_) {
		if (linePos_.empty()) {
			node.isConect_ = false;
			continue;
		}

		for (auto& pos : linePos_) {
			if ((node.GetMoveToPos() == pos.first || node.GetMoveToPos() ==pos.second)) {
				node.isConect_ = true;
				break;
			}
			else {
				node.isConect_ = false;
			}
		}
	}

	if (portCount_ > linePos_.size() * 2) {
		isLineConect_ = true;
	}
	else if (portCount_ < linePos_.size() * 2) {
		isLineDisconect_ = true;
	}
	else {
		isLineConect_ = false;
		isLineDisconect_ = false;
	}

	portCount_ = linePos_.size() * 2;

	if (portCount_ >= allPortCount_) 
	{
		isCanGoal_ = true;
		goal_->isCanGoal_ = isCanGoal_;
	}
	else {
		isCanGoal_ = false;
		goal_->isCanGoal_ = isCanGoal_;
	}
}

void Map::MapReset() {
	if (!isClear_ && !isDie_) {
		for (auto &i : nodes_) {
			i.SetStartPos();
		}

		if (player_) {
			player_->SetStartPos();
		}

		mapDatas_ = startMapDatas_;

		linePos_.clear();

		for (auto &i : lines_) {
			i.SetPos(Vector2::kZero, Vector2::kZero, kMapSize);
		}

		Lines();

		isCanGoal_ = false;
		isClear_ = false;
		isDie_ = false;
		//isGoalAnimationFinish_ = false;
	}
}

void Map::Undo(bool isMove) {
	if (!isDie_ && !isClear_ && !isMove && !player_->GetIsEase() &&
		(Gamepad::GetInstance()->Pushed(Gamepad::Button::B) || KeyInput::GetInstance()->Pushed(DIK_Z))) {
		for (auto &i : nodes_) {
			i.SetPrePos();
			mapDatas_[int(i.GetMapNum().y)][int(i.GetMapNum().x)] = int(i.GetIsPort().to_ulong());
		}
		if (player_) {
			player_->SetPrePos();

			mapDatas_[int(player_->mapPos_.y)][int(player_->mapPos_.x)] = int(Type::kPlayer);
		}
		Lines();
	}
}

Map::~Map()
{
	nodes_.clear();
	mapDatas_.clear();
}